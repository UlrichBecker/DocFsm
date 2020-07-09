/*****************************************************************************/
/*                                                                           */
/*!             @brief Command line interface of DOC-FSM                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_commandline.hpp                                              */
/*! @see     df_commandline.cpp                                              */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates: 09.07.2020  generate_doc_tagged                                 */
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

#ifdef CONFIG_GSI_AUTODOC_OPTION
   /*!
    * @brief GSI specific option for Autodoc its only necessary when this tool
    *        will used in the GSI (www.gsi.de).
    */
   class OptGsiAutodoc: public CLOP::OPTION_V
   {
   public:
      OptGsiAutodoc( void );
      int onGiven( PARSER* poParser ) override;
   };
#endif

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

   OptPrintHelp  m_optPrintHelp;
#ifdef CONFIG_GSI_AUTODOC_OPTION
   OptGsiAutodoc m_optGsiAutodoc;
#endif
   OptVerbose    m_optVerbose;
   OptVersion    m_optVersion;

   FILE_NAME_LIST_T m_vSourceFiles;
   bool             m_verbose;

public:
   CommandlineParser( int argc, char** ppArgv )
      :PARSER( argc, ppArgv )
      ,m_verbose(false)
   {
      add( m_optPrintHelp )
#ifdef CONFIG_GSI_AUTODOC_OPTION
      ( m_optGsiAutodoc )
#endif
      ( m_optVerbose )( m_optVersion );
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
