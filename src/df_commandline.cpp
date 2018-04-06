/*****************************************************************************/
/*                                                                           */
/*!             @brief Command line interface of CSTATE                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_commandline.cpp                                              */
/*! @see     df_commandline.hpp                                              */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef __DOCFSM__
 #include <unistd.h>
#endif
#include "df_docfsm.hpp"
#include "df_commandline.hpp"

using namespace CLOP;
using namespace DocFsm;

/*!----------------------------------------------------------------------------
*/
CommandlineParser::OptPrintHelp::OptPrintHelp( void )
{
   m_hasArg   = NO_ARG;
   m_shortOpt = 'h';
   m_longOpt  = "help";
   m_helpText = "Print this help and exit.";
}

/*!----------------------------------------------------------------------------
*/
int CommandlineParser::OptPrintHelp::onGiven( PARSER* poParser )
{
   std::cout << "\nDocFsm is a documentation and reverse engineering tool for graphical presentation of\n"
                "finite state machines (FSM) from C and/or C++ sourcefiles.\n\n";
   std::cout << "It translates C/C++ sourcefiles containing a FSM - respectively containing\n"
                "for the FSM defined macros - into the DOT-language.\n\n";
   std::cout << "(c) 2017 Ulrich Becker\n\n"; 
   std::cout << "Usage: " << poParser->getProgramName() << " [options,...] <C/C++ sourcefile [C/C++ sourcefile ...]>\n\n";

   std::cout << "Example 1: Creating a dot-file from a C sourcefile:\n";
   std::cout << poParser->getProgramName() <<
                " myFsm.c > myFsm.gv\n\n";

   std::cout << "Example 2: Creating a PDF from a C++11 sourcefile with blue and thick transitions:\n";
   std::cout << poParser->getProgramName() <<
                " -E style=bold -E color=blue"
                " --std c++11 -I /path/to/my/additional/headers myFsm.cpp"
                " | dot -Tpdf -o myFsm.pdf\n\n";

   std::cout << "Options:\n";
   poParser->list( std::cout );
   std::cout << std::endl;
   ::exit( EXIT_SUCCESS );
   return 0;
} 

/*!----------------------------------------------------------------------------
*/
CommandlineParser::OptVerbose::OptVerbose( void )
{
   m_hasArg   = NO_ARG;
   m_shortOpt = 'v';
   m_longOpt  = "verbose";
   m_helpText = "Be verbose.";
}

/*!----------------------------------------------------------------------------
*/
int CommandlineParser::OptVerbose::onGiven( PARSER* poParser )
{
   static_cast<CommandlineParser*>(poParser)->m_verbose = true;
   return 0;
}

/*!----------------------------------------------------------------------------
*/
CommandlineParser::OptVersion::OptVersion( void )
{
   m_hasArg   = NO_ARG;
   m_shortOpt = 'V';
   m_longOpt  = "version";
   m_helpText = "Print version and exit.";
}

/*!----------------------------------------------------------------------------
*/
int CommandlineParser::OptVersion::onGiven( PARSER* poParser )
{
#ifdef _DEBUG
  #define VERSION_STRING  DOCFSM_VERSION " Debug"
#else
  #define VERSION_STRING  DOCFSM_VERSION " Release"
#endif
   std::cout << VERSION_STRING << std::endl;
   ::exit( EXIT_SUCCESS );
   return 0;
}

/*!----------------------------------------------------------------------------
*/
int CommandlineParser::onArgument( void )
{
   if( ::access( getArgVect()[getArgIndex()], F_OK ) != 0 )
   {
      ERROR_MESSAGE( "File: \"" << getArgVect()[getArgIndex()] << "\" not found!" );
      ::exit( EXIT_FAILURE );
      return -1;
   }
   m_vSourceFiles.push_back( getArgVect()[getArgIndex()] );
   return 1; // That means continue in the command-line parsing...
}

//================================== EOF ======================================
