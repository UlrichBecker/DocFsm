/*****************************************************************************/
/*                                                                           */
/*!  @brief Module prepares the C/C++-soucecode after the preprocessor run.  */
/*                                                                           */
/*         It removes comments quoted strings and macro-defines              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_preparser.cpp                                                */
/*! @see     df_preparser.hpp                                                */
/*! @author  Ulrich Becker                                                   */
/*! @date    05.11.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_preparser.hpp"

using namespace DocFsm;

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::State::__transition( State& rNewState )
{
    assert( dynamic_cast<State*>(&rNewState) != nullptr );

    if( m_pParent->m_pCurrentState == &rNewState )
    {
        return false;
    }

    DEBUG_FSM_MESSAGE( typeid(*m_pParent->m_pCurrentState).name()
                       << " -> " << typeid(rNewState).name() );

    m_pParent->m_pCurrentState = &rNewState;

    return rNewState.onEntry();
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::InCode::onEntry( void )
{
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::InCode::onDo( char c )
{
   switch( c )
   {
#if 0
      case '\\':
      {
         if( m_pParent->m_lastChar != '\\' )
            return false;
         break;
      }
#endif
      case '#':
      {
         if( m_pParent->m_lastChar != '\'' && m_pParent->m_lastChar != '\\')
         {
            FSM_TRANSITION( InMacro, label= '\'\#\' recognized' );
         }
         break;
      }
      case '"':
      {
         if( m_pParent->m_lastChar != '\'' )
            FSM_TRANSITION( InQuotes, label = 'Quotes recognized' );
         break;
      }
      case '/':
      {
         if( m_pParent->m_lastChar != '\\' )
            FSM_TRANSITION( StartComment, label='\'\/\' recognized' );
      }
   }
   m_pParent->m_rOutput += c;
   FSM_TRANSITION( InCode );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::InQuotes::onEntry( void )
{
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::InQuotes::onDo( char c )
{
   if( (c == '"') && (m_pParent->m_lastChar != '\\') )
      FSM_TRANSITION( InCode, label = 'Second quotes\nrecognized' );

   FSM_TRANSITION( InQuotes );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::StartComment::onEntry( void )
{
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::StartComment::onDo( char c )
{
   switch( c )
   {
      case '*' : FSM_TRANSITION( InBlockComment, label='\'*\' recognized' );
      case '/' : FSM_TRANSITION( InLineComment, label='\'\/\' recognized' );
   }
   FSM_TRANSITION( InCode );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::InLineComment::onEntry( void )
{
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::InLineComment::onDo( char c )
{
   if( c == '\n' )
   {
      m_pParent->m_rOutput += c;
      FSM_TRANSITION( InCode, label='LF recognized' );
   }
   FSM_TRANSITION( InLineComment );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::InBlockComment::onEntry( void )
{
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::InBlockComment::onDo( char c )
{
   if( c == '\n' )
   {
      m_pParent->m_rOutput += c;
      return false;
   }
   if( (c == '/') && (m_pParent->m_lastChar == '*') )
      FSM_TRANSITION( InCode, label='\'\'*\/\'\' recognized' );

   FSM_TRANSITION( InBlockComment );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool Preparser::InMacro::onEntry( void )
{
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::InMacro::onDo( char c )
{
   if( c == '\n' )
   {
      m_pParent->m_rOutput += c;
      if( m_pParent->m_lastChar != '\\')
         FSM_TRANSITION( InCode, label='LF recognized' );
   }

   FSM_TRANSITION( InMacro, label='backslash recognized');
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
Preparser::Preparser( std::istream& rInput, std::string& rOutput )
       :FSM_INIT_STATE( InCode )
       ,FSM_INIT_STATE( InQuotes )
       ,FSM_INIT_STATE( StartComment )
       ,FSM_INIT_STATE( InLineComment )
       ,FSM_INIT_STATE( InBlockComment )
       ,FSM_INIT_STATE( InMacro )
       ,FSM_INIT_FSM( InCode, label='Start', color=blue, fontcolor=blue )
       ,m_lastChar( '\0' )
       ,m_rOutput( rOutput )
       ,m_rInput( rInput )
{
   m_rOutput.clear();
}

/*!----------------------------------------------------------------------------
*/
bool Preparser::remove( void )
{
   std::for_each( std::istreambuf_iterator<char>(m_rInput),
                  std::istreambuf_iterator<char>(),
                  [this] (char c) -> void
                  {
                     m_pCurrentState->onDo( c );
                     m_lastChar = c;
                  });
   return false;
}

//================================== EOF ======================================
