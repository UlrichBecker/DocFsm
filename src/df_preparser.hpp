/*****************************************************************************/
/*                                                                           */
/*! @brief Module prepares the C/C++-soucecode after the preprocessor run.   */
/*                                                                           */
/*         It removes comments quoted strings and macro-defines              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_preparser.hpp                                                */
/*! @see     df_preparser.cpp                                                */
/*! @author  Ulrich Becker                                                   */
/*! @date    05.11.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_PREPARSER_HPP
#define _DF_PREPARSER_HPP

#ifndef __DOCFSM__ // Accelerates DocFsm
  #include <iterator>
  #include <iostream>
  #include <algorithm>
  #include <assert.h>
  #include <sstream>
#endif

#ifdef _DEBUG_PREPARSER
  #define  _FSM_DEBUG_INFO
#endif
#include "fsmhelper.hpp"

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
/*!
 * @dotfile df_preparser.gv
 */
class Preparser
{
   class State: public FSM_STATE< Preparser, char >
   {
       friend class Preparser;
       State( Preparser* pParent ): FSM_STATE( pParent ) {}
       bool __transition( State& rNewState );
   };

   #define FSM_DECLARE_STATE( name, attr... ) _FSM_DECLARE_STATE( Preparser, name )
   FSM_DECLARE_STATE( InCode, style=bold, color=blue );
   FSM_DECLARE_STATE( InQuotes, color=red );
   FSM_DECLARE_STATE( StartComment, color=gray );
   FSM_DECLARE_STATE( InLineComment, color=gray );
   FSM_DECLARE_STATE( InBlockComment, color=gray );
   FSM_DECLARE_STATE( InMacro, color=green );
   State*           m_pCurrentState;
   char             m_lastChar;
   std::string&     m_rOutput;
   std::istream&    m_rInput;

public:
   Preparser( std::istream& rInput, std::string& rOutput );
   bool remove( void );
   bool operator()( void )
   {
      return remove();
   }
};

} // End namespace DocFsm
#endif // ifndef _DF_PREPARSER_HPP
//================================== EOF ======================================
