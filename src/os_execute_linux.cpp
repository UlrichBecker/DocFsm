/*****************************************************************************/
/*                                                                           */
/*!              @brief Module invokes a Linux executable file               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    os_execute_linux.cpp                                            */
/*! @see     os_execute_linux.hpp                                            */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef __linux__
  #error This module is for Linux only!
#endif
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "messages.hpp"
#include "os_execute_linux.hpp"

using namespace OS; 

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/

Execution::Execution( std::ostream& rStdOstream, std::ostream& rErrOstream  )
   :m_rStdOstream( rStdOstream )
   ,m_rErrOstream( rErrOstream )
{
}

/*!----------------------------------------------------------------------------
*/
Execution::~Execution( void )
{
}

/*!----------------------------------------------------------------------------
*/
bool Execution::run( const std::string& rFilename )
{
   if( ::access( m_exe.c_str(), X_OK ) != 0 )
   {
      ERROR_MESSAGE( "Executable binary not found in \"" << m_exe
                     << "\" " << ::strerror( errno ) );
      return true;
   }

   if( m_inPipe() )
   {
      ERROR_MESSAGE( "Couldn't create input-pipe! " << ::strerror( errno ) );
      return true;
   }

   if( m_outPipe() )
   {
      ERROR_MESSAGE( "Couldn't create stdout-pipe! " << ::strerror( errno ) );
      return true;
   }

   if( m_errPipe() )
   {
      ERROR_MESSAGE( "Couldn't create stederr-pipe! " << ::strerror( errno ) );
      return true;
   }

   m_pid = ::fork();
   if( m_pid < 0 )
   {
      ERROR_MESSAGE( "Couldn't fork to process \"" << m_exe << "\" "
                    << ::strerror( errno ) );
      return true;
   }

   bool ret = false;
   if( m_pid == 0 )
      runChildProcess( rFilename );
   else
      ret = runParentProcess();

   ::close( m_inPipe.fd.read );
   ::close( m_outPipe.fd.write );
   ::close( m_errPipe.fd.write );

   return ret;
}

/*!----------------------------------------------------------------------------
*/
bool Execution::runParentProcess( void )
{
   int fdMax = m_outPipe.fd.read;
   if( fdMax < m_errPipe.fd.read )
      fdMax = m_errPipe.fd.read;
   fdMax++;

   fd_set rfds;
   int selectStade;
   bool ret = false;
   do
   {
      ssize_t size;
      char buffer[1024];

      FD_ZERO( &rfds );
      FD_SET( m_outPipe.fd.read, &rfds );
      FD_SET( m_errPipe.fd.read, &rfds );
      struct timeval testIntervall = {0, 10};
      selectStade = ::select( fdMax, &rfds, nullptr, nullptr, &testIntervall );
      if( selectStade == 0 )
      {
         int processState;
         if( m_pid == ::waitpid( m_pid, &processState, WNOHANG ) )
         {
            int exitStatus = WEXITSTATUS( processState );
            if( exitStatus != EXIT_SUCCESS )
            {
               ERROR_MESSAGE( m_exe << " returns by state: " << exitStatus );
               ret = true;
            }
            break;
         }
      }
      else if( selectStade > 0 )
      {
         if( FD_ISSET( m_outPipe.fd.read, &rfds ) )
         {
            size = ::read( m_outPipe.fd.read, buffer, sizeof(buffer)-1 );
            if( size < 0 )
            {
               ERROR_MESSAGE( "Reading stdout-pipe: " << ::strerror( errno ) );
               ret = true;
               break;
            }
            else
            {
               assert( size < sizeof(buffer) );
               buffer[size] = '\0';
               m_rStdOstream << buffer;
            }
         }
         if( FD_ISSET( m_errPipe.fd.read, &rfds ) )
         {
            size = ::read( m_errPipe.fd.read, buffer, sizeof(buffer)-1 );
            if( size < 0 )
            {
               ERROR_MESSAGE( "Reading stderr-pipe: " << ::strerror( errno ) );
               ret = true;
               break;
            }
            else if( size > 0 )
            {
               assert( size < sizeof(buffer) );
               buffer[size] = '\0';
               m_rErrOstream << buffer;
            }
         }
      }
   }
   while( selectStade >= 0 );

   return ret;
}

/*!----------------------------------------------------------------------------
*/
bool Execution::runChildProcess( const std::string& rFilename )
{
   if( ::dup2( m_inPipe.fd.read, STDIN_FILENO ) < 0 )
   {
      ERROR_MESSAGE( "Couldn't duplicate stdin-pipe: " << ::strerror( errno ) );
      return true;
   }
   if( ::dup2( m_outPipe.fd.write, STDOUT_FILENO ) < 0 )
   {
      ERROR_MESSAGE( "Couldn't duplicate stdout-pipe: " << ::strerror( errno ) );
      return true;
   }
   if( ::dup2( m_errPipe.fd.write, STDERR_FILENO ) < 0 )
   {
      ERROR_MESSAGE( "Couldn't duplicate stderr-pipe: " << ::strerror( errno ) );
      return true;
   }

   ::close( m_inPipe.fd.read );
   ::close( m_inPipe.fd.write );
   ::close( m_outPipe.fd.read );
   ::close( m_outPipe.fd.write );
   ::close( m_errPipe.fd.read );
   ::close( m_errPipe.fd.write );

   m_oOptionList.push_back( rFilename );
   m_oOptionList.insert( m_oOptionList.begin(), m_exe );

   std::vector<CHAR_STRING_T> argList( m_oOptionList.begin(), m_oOptionList.end() );
   std::vector<char*> argv( argList.begin(), argList.end() );
   argv.push_back( nullptr );

#ifdef _DEBUG
   for( int i = 0; argv[i] != nullptr; i++ )
      std::cerr << argv[i] << std::endl;
#endif
   if( ::execvp( argv[0], &argv[0] ) < 0 )
   {
      ERROR_MESSAGE( "Could not execute \"" << m_exe << "\" "
                     << ::strerror( errno ) );
      return true;
   }
   return false;
}

//================================== EOF ======================================
