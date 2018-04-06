/*****************************************************************************/
/*                                                                           */
/*!           @brief Module calls the C/C++ preprocessor "cpp"               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_cpp_caller.hpp                                               */
/*! @see     df_cpp_caller.cpp                                               */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_CPP_CALLER_HPP
#define _DF_CPP_CALLER_HPP

#ifndef __DOCFSM__
 #include "os_execute.hpp"
 #include "df_commandline.hpp"
#endif

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
class CppCaller: public OS::Execution
{
   class CppOption: public CLOP::OPTION_V
   {
   protected:
      CppCaller*   m_pParent;
      CppOption( CppCaller* pCppCaller )
         :m_pParent( pCppCaller )
      {
         m_hasArg = REQUIRED_ARG;
      }
   };

   class CppOptionInclude: public CppOption
   {
   public:
      CppOptionInclude( CppCaller* pCppCaller );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class CppOptionDefine: public CppOption
   {
   public:
      CppOptionDefine( CppCaller* pCppCaller );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class CppOptionUndefine: public CppOption
   {
   public:
      CppOptionUndefine( CppCaller* pCppCaller );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class CppOptionStd: public CppOption
   {
   public:
      CppOptionStd( CppCaller* pCppCaller );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class CppOptionCallPath: public CppOption
   {
   public:
      CppOptionCallPath( CppCaller* pCppCaller );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   CppOptionInclude    m_oOptionInclude;
   CppOptionDefine     m_oOptionDefine;
   CppOptionUndefine   m_oOptionUndefine;
   CppOptionStd        m_oCppOptionStd;
   CppOptionCallPath   m_oOptionCallPath;

public:
   CppCaller( std::ostream& rOstream, CommandlineParser& rParser );
   ~CppCaller( void )
   {
   }
};

} // End namespace DocFsm

#endif // ifndef _DF_CPP_CALLER_HPP
//================================== EOF ======================================
