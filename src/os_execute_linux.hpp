/*****************************************************************************/
/*                                                                           */
/*!              @brief Module invokes a Linux executable file               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    os_execute_linux.hpp                                            */
/*! @see     os_execute_linux.cpp                                            */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _OS_EXECUTE_LINUX_HPP
#define _OS_EXECUTE_LINUX_HPP
#ifndef __linux__
  #error This module is for Linux only!
#endif

#include <assert.h>
#include <unistd.h>
#include <string>
#include <vector>

namespace OS
{

///////////////////////////////////////////////////////////////////////////////
class Execution
{
   typedef std::vector<std::string> OPTION_LIST_T;

   struct CHAR_STRING_T
   {
      mutable std::vector<char> m_buf;

         CHAR_STRING_T( std::string s )
         :m_buf( s.c_str(), s.c_str() + s.size() + 1 )
      {}

      operator char*( void ) const
      {
          return &m_buf[0];
      }
   };

   union PIPE_T
   {
      struct FD_T
      {
         int read;
         int write;
      };

   private:

      int  i[2];

   public:

      static_assert( sizeof( i ) == sizeof( FD_T ), "Size-error!" );

      FD_T fd;

      PIPE_T( void )
      {
         assert( &fd.read  == &i[0] );
         assert( &fd.write == &i[1] );
      }

      bool operator()( void )
      {
         return (::pipe( i ) < 0);
      }
   };

   pid_t                     m_pid;
   PIPE_T                    m_inPipe;
   PIPE_T                    m_outPipe;
   PIPE_T                    m_errPipe;
   std::ostream&             m_rStdOstream;
   std::ostream&             m_rErrOstream;

protected:
   std::string               m_exe;
   OPTION_LIST_T             m_oOptionList;

   Execution( std::ostream& rStdOstream, std::ostream& rErrOstream  );
   ~Execution( void );

public:
   bool run( const std::string& rFilename );

private:
   bool runParentProcess( void );
   bool runChildProcess( const std::string& rFilename );
};

} // namespace OS
#endif // ifndef _OS_EXECUTE_LINUX_HPP
//================================== EOF ======================================
