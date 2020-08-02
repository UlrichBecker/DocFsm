/*****************************************************************************/
/*                                                                           */
/*!             @brief Command line interface of DOC-FSM                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_commandline.cpp                                              */
/*! @see     df_commandline.hpp                                              */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates: 09.07.2020  generate_doc_tagged                                                             */
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
   std::cout << "(c) 2017 - 2020 Ulrich Becker\n\n"; 
   std::cout << "Usage: " << poParser->getProgramName() << " [options,...] <C/C++ sourcefile [C/C++ sourcefile ...]>\n\n";

   std::cout << "Example 1: Creating a dot-file from a C sourcefile:\n";
   std::cout << poParser->getProgramName() <<
                " myFsm.c > myFsm.gv\n\n";

   std::cout << "Example 2: Creating a PDF from a C++11 sourcefile with blue and thick transitions:\n";
   std::cout << poParser->getProgramName() <<
                " -E style=bold -E color=blue"
                " --std c++11 -I /path/to/my/additional/headers myFsm.cpp"
                " | dot -Tpdf -o myFsm.pdf\n\n";

   std::cout << "Example 3: Displaying directly from source file via Image Magick:\n";
   std::cout << poParser->getProgramName() <<
                " myFsm.cpp | display\n\n";

   std::cout << "Options:\n";
   poParser->list( std::cout );
   std::cout << std::endl;
   ::exit( EXIT_SUCCESS );
   return 0;
}

#ifdef CONFIG_GSI_AUTODOC_OPTION
/*!----------------------------------------------------------------------------
*/
CommandlineParser::OptGsiAutodoc::OptGsiAutodoc( void )
{
   m_hasArg   = NO_ARG;
   m_shortOpt = '\0';
   m_longOpt  = "generate_doc_tagged";
   m_helpText = "GSI specific option will used from GSI-Autodoc only. (www.gsi.de)";
}

/*!----------------------------------------------------------------------------
*/
int CommandlineParser::OptGsiAutodoc::onGiven( PARSER* poParser )
{
   std::string name = poParser->getProgramName().substr(poParser->getProgramName().find_last_of('/')+1);
   std::cout 
   << "<toolinfo>\n"
      "\t<name>" << name << "</name>\n"
      "\t<topic>Development, Release, Rollout</topic>\n"
      "\t<description>Documentation and develop tool for visualizing"
                      " of Finite State Machines from source code written"
                      " in C or C++, via Graphviz and/or Image Magick.</description>\n"
      "\t<usage>" << name;
      for( const auto& pOption: *poParser )
      {
         if( pOption == this )
            continue;
         std::cout << " [";
         if( pOption->m_shortOpt != '\0' )
         {
            std::cout << '-' << pOption->m_shortOpt;
            if( pOption->m_hasArg != OPTION::NO_ARG )
            {
               std::cout << ' ';
            #ifndef CONFIG_CLOP_NO_OPTIONAL_ARG
               if( pOption->m_hasArg == OPTION::OPTIONAL_ARG )
                  std::cout << '=';
            #endif
               std::cout << "ARG";
            }
            if( !pOption->m_longOpt.empty() )
               std::cout << ", ";
         }
         if( !pOption->m_longOpt.empty() )
         {
            std::cout << "--" << pOption->m_longOpt;
            if( pOption->m_hasArg != OPTION::NO_ARG )
            {
               std::cout << ' ';
            #ifndef CONFIG_CLOP_NO_OPTIONAL_ARG
               if( pOption->m_hasArg == OPTION::OPTIONAL_ARG )
                   std::cout << '=';
            #endif
               std::cout << "ARG";
            }
         }
         std::cout << ']';
      }
      std::cout << " {soucefile(s)}\n\t</usage>\n"
      "\t<author>Ulrich Becker</author>\n"
      "\t<tags>graphics</tags>\n"
      "\t<version>" DOCFSM_VERSION "</version>\n"
      "\t<documentation>https://github.com/UlrichBecker/DocFsm</documentation>\n"
      "\t<environment></environment>\n"
      "\t<requires>Graphviz and/or Image Magick</requires>\n"
      "\t<autodocversion>1.0</autodocversion>\n"
   "</toolinfo>" << std::endl;
   ::exit( EXIT_SUCCESS );
   return 0;
}
#endif // ifdef CONFIG_GSI_AUTODOC_OPTION

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
