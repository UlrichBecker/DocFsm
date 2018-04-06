/*****************************************************************************/
/*                                                                           */
/*!           @brief Module calls the C/C++ preprocessor "cpp"               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_cpp_caller.cpp                                               */
/*! @see     df_cpp_caller.hpp                                               */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_docfsm.hpp"
#include "df_cpp_caller.hpp"

using namespace DocFsm;

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
CppCaller::CppOptionInclude::CppOptionInclude( CppCaller* pCppCaller )
   :CppOption( pCppCaller )
{
   m_shortOpt  = 'I';
   m_longOpt   = "include";
   m_helpText  = "Add the directory PARAM to the list of directories"
                 " to be searched for header files.\n"
                 "This option will forwarded directly to the preprocessor as"
                 " option \"-I\"";
}

/*!----------------------------------------------------------------------------
*/
int CppCaller::CppOptionInclude::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_oOptionList.push_back( "-I" + poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
CppCaller::CppOptionDefine::CppOptionDefine( CppCaller* pCppCaller )
   :CppOption( pCppCaller )
{
   m_shortOpt  = 'D';
   m_longOpt   = "define";
   m_helpText  = "Predefine PARAM as a macro, with definition 1\n"
                 "A already predfined macro is __DOCFSM__\n"
                 "This option will forwarded directly to the preprocessor as"
                 " option \"-D\"";
}

/*!----------------------------------------------------------------------------
*/
int CppCaller::CppOptionDefine::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_oOptionList.push_back( "-D" + poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
CppCaller::CppOptionUndefine::CppOptionUndefine( CppCaller* pCppCaller )
   :CppOption( pCppCaller )
{
   m_shortOpt = 'U';
   m_longOpt  = "undefine";
   m_helpText = "Cancel any previous definition of PARAM, either"
                " built in or provided with a -D option.\n"
                "This option will forwarded directly to the preprocessor as"
                " option \"-U\"";
}

/*!----------------------------------------------------------------------------
*/
int CppCaller::CppOptionUndefine::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_oOptionList.push_back( "-U" + poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
CppCaller::CppOptionStd::CppOptionStd( CppCaller* pCppCaller )
   :CppOption( pCppCaller )
{
   m_shortOpt = '\0';
   m_longOpt  = "std";
   m_helpText = "Specify the standard to which the code should conform.\n"
                "This option will forwarded directly to the preprocessor as"
                " option \"-std=\"";
}

/*!----------------------------------------------------------------------------
*/
int CppCaller::CppOptionStd::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_oOptionList.push_back( "-std=" + poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
CppCaller::CppOptionCallPath::CppOptionCallPath( CppCaller* pCppCaller )
   :CppOption( pCppCaller )
{
   m_shortOpt  = 'p';
   m_longOpt   = "cpp-path";
   m_helpText  = "Replaces the default preprocessor \"cpp\" by the in PARAM named"
                 " preprocessor.\n"
                 "This could be necessary for cross-compiling.";
}

/*!----------------------------------------------------------------------------
*/
int CppCaller::CppOptionCallPath::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_exe = poParser->getOptArg();
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
CppCaller::CppCaller( std::ostream& rOstream, CommandlineParser& rParser )
   :Execution( rOstream, std::cerr )
   ,m_oOptionInclude( this )
   ,m_oOptionDefine( this )
   ,m_oOptionUndefine( this )
   ,m_oCppOptionStd( this )
   ,m_oOptionCallPath( this )
{
   rParser( m_oOptionInclude )( m_oOptionDefine )( m_oOptionUndefine )
          ( m_oCppOptionStd )( m_oOptionCallPath );
   m_exe = "/usr/bin/cpp";
   m_oOptionList.push_back( "-P" );
   m_oOptionList.push_back( "-fdirectives-only" );
   m_oOptionList.push_back( "-D__DOCFSM__" );
}

//================================== EOF ======================================
