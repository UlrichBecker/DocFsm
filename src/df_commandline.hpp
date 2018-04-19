/*****************************************************************************/
/*                                                                           */
/*!             @brief Command line interface of CSTATE                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_commandline.hpp                                              */
/*! @see     df_commandline.cpp                                              */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_COMMANDLINE_HPP
#define _DF_COMMANDLINE_HPP

#ifndef __DOCFSM__
 #include "parse_opts.hpp"
#endif

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
class CommandlineParser: public CLOP::PARSER
{
   typedef std::vector<std::string> FILE_NAME_LIST_T;

   class OptPrintHelp: public CLOP::OPTION_V
   {
   public:
      OptPrintHelp( void );
      int onGiven( PARSER* poParser ) override;
   };

   class OptVerbose: public CLOP::OPTION_V
   {
   public:
      OptVerbose( void );
      int onGiven( PARSER* poParser ) override;
   };

   class OptVersion: public CLOP::OPTION_V
   {
   public:
      OptVersion( void );
      int onGiven( PARSER* poParser ) override;
   };

   OptPrintHelp m_optPrintHelp;
   OptVerbose   m_optVerbose;
   OptVersion   m_optVersion;

   FILE_NAME_LIST_T m_vSourceFiles;
   bool             m_verbose;

public:
   CommandlineParser( int argc, char** ppArgv )
      :PARSER( argc, ppArgv )
      ,m_verbose(false)
   {
      add( m_optPrintHelp )( m_optVerbose )( m_optVersion );
   }

   int onArgument( void ) override;

   const FILE_NAME_LIST_T& getFileNameList( void ) const
   {
      return m_vSourceFiles;
   }

   bool isVerbose( void ) const
   {
      return m_verbose;
   }
};

} // End namespace DocFsm
#endif // ifndef _DF_COMMANDLINE_HPP
//================================== EOF ======================================
