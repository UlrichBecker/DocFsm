/*****************************************************************************/
/*                                                                           */
/*!        @brief Base-module for recognizing DOCFSM-keywords                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_keyword.cpp                                                  */
/*! @see     df_keyword.hpp                                                  */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef __DOCFSM__
 #include <iterator>
 #include <algorithm>
 #include <iostream>
 #include <assert.h>
#endif

#include "df_keyword.hpp"

using namespace DocFsm;

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
Keyword::Keyword( void )
   : m_lastChar( '\0' )
{
   m_oCurrentWord.clear();
}

/*!----------------------------------------------------------------------------
*/
bool Keyword::isKeywordChar( const char c )
{
   if( c >= 'a' && c <= 'z' )
      return true;
   if( c >= 'A' && c <= 'Z' )
      return true;
   if( c >= '0' && c <= '9' )
      return true;
   if( c == '_' )
      return true;

   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Keyword::isFillChar( const char c )
{
   switch( c )
   {
      case '\n':
      case '\r':
      case '\t':
      case ' ':
      {
         return true;
      }
   }
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Keyword::addIfBelongsToKeyword( const char c )
{
    if( isKeywordChar( c ) )
    {
       m_oCurrentWord += c;
       return true;
    }
    if( m_oCurrentWord.empty() )
       return false;

    onRecognized( m_oCurrentWord );
    m_oCurrentWord.clear();

    return false;
}

/*!----------------------------------------------------------------------------
*/
void Keyword::_onChar( char c )
{
   onChar( c );
   m_lastChar = c;
}

///////////////////////////////////////////////////////////////////////////////

/*!----------------------------------------------------------------------------
*/
SourceBrowser::SourceBrowser( std::string& rsSource )
   :m_rsSource( rsSource )
{
}

/*!----------------------------------------------------------------------------
*/
void SourceBrowser::set( Keyword* pInterpreter )
{
   assert( m_stack.size() < 2 );
   m_stack.push( pInterpreter );
}

/*!----------------------------------------------------------------------------
*/
void SourceBrowser::back( void )
{
   assert( m_stack.size() > 0 );
   m_stack.pop();
}

/*!----------------------------------------------------------------------------
*/
bool SourceBrowser::browse( void )
{
   for( auto& i : m_rsSource )
   {
      assert( m_stack.size() > 0 );
      assert( m_stack.size() <= 2 );
      m_stack.top()->_onChar( i );
   }
   return false;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool KeywordInterpreter::browse( void )
{
   m_rSourceBrowser.set( this );
   bool ret = m_rSourceBrowser.browse();
   m_rSourceBrowser.back();
   return ret;
}

/*!----------------------------------------------------------------------------
*/
void KeywordInterpreter::onChar( char c )
{
   addIfBelongsToKeyword( c );
}

//================================== EOF ======================================
