/*****************************************************************************/
/*                                                                           */
/*!              @brief Sub-FSM for reading DOT-attributes                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_attribute_reader.cpp                                         */
/*! @see     df_attribute_reader.hpp                                         */
/*! @author  Ulrich Becker                                                   */
/*! @date    02.01.2018                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_docfsm.hpp"
#include "df_attribute_reader.hpp"

using namespace DocFsm;

/*!----------------------------------------------------------------------------
*/
AttributeReader::AttributeReader( void )
   :m_pAttrList(nullptr)
   ,m_poSourceBrowser(nullptr)
   ,m_pDotKeywords(nullptr)
   ,m_currentChar( '\0' )
   ,m_currentDotKeyword( nullptr )
   ,FSM_INIT_OBJ( AttributeReader, ReadDotKeyword )
   ,FSM_INIT_OBJ( AttributeReader, ReadEqualChar )
   ,FSM_INIT_OBJ( AttributeReader, SkipOverFillCharacters )
   ,FSM_INIT_OBJ_X( AttributeReader, ReadValue )
{}

/*!----------------------------------------------------------------------------
*/
void AttributeReader::start( SourceBrowser& roSorceBrowser,
                             const DotKeywords::DOT_ATTR_LIST_T& roDotKeywords,
                             ATTR_LIST_T& rAttrList )
{
   m_poSourceBrowser = &roSorceBrowser;
   m_pDotKeywords    = &roDotKeywords;
   m_pAttrList       = &rAttrList;
   m_poSourceBrowser->set( this );
   FSM_INIT_FSM( ReadDotKeyword, color=red, label='Start' );
}

/*!----------------------------------------------------------------------------
*/
void AttributeReader::onRecognized( std::string word )
{
   assert( dynamic_cast<STATE_T*>(m_pCurrentState) != nullptr );
   m_strCurrentWord = word;
   fsmDo( WORD );
}

/*!----------------------------------------------------------------------------
*/
void AttributeReader::onChar( char c )
{
   assert( dynamic_cast<STATE_T*>(m_pCurrentState) != nullptr );
   m_currentChar = c;

   if( FSM_IS(ReadDotKeyword) && addIfBelongsToKeyword( c ) )
      return;

   fsmDo( CHAR );
}

/*!----------------------------------------------------------------------------
*/
void AttributeReader::fsmTransition( STATE_T& rNewState )
{
   assert( dynamic_cast<STATE_T*>(m_pCurrentState) != nullptr );

   if( m_pCurrentState == &rNewState )
      return;
#ifdef _DEBUG_FSM_IMPLEMENTER
   std::cerr << m_pCurrentState->m_name << " -> " << rNewState.m_name << std::endl;
#endif
   if( m_pCurrentState->m_onExit != nullptr )
      CALL_MEMBER_FUNC( m_pCurrentState->m_onExit )();

   m_pCurrentState = &rNewState;

   if( m_pCurrentState->m_onEntry != nullptr )
      CALL_MEMBER_FUNC( m_pCurrentState->m_onEntry )();
}

/*!----------------------------------------------------------------------------
*/
void AttributeReader::fsmReturn( void )
{
   assert( dynamic_cast<STATE_T*>(m_pCurrentState) != nullptr );

   if( m_pCurrentState->m_onExit != nullptr )
      CALL_MEMBER_FUNC( m_pCurrentState->m_onExit )();

   m_poSourceBrowser->back();
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
bool AttributeReader::ReadDotKeyword( EVENT_T ev )
{
   if( ev == CHAR )
   {
      if( m_currentChar == ')' )
      {
         FSM_RETURN( label='\')\' recognized\nReady'  );
         return false;
      }
      return false;
   }
   assert( ev == WORD );
   FSM_TRANSITION( ReadEqualChar );
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool AttributeReader::ReadEqualChar( EVENT_T ev )
{
   if( ev == WORD )
   {
      FSM_RETURN( label='Error' );
      return true;
   }
   assert( ev == CHAR );
   if( m_currentChar == ',' )
   {
      FSM_TRANSITION( ReadDotKeyword, label='\',\' recognized\n'
                      'last argument wasn\'t a DOT-attribute.' );
      return false;
   }
   if( m_currentChar == '=' )
   {
      m_currentDotKeyword = DotKeywords::find( *m_pDotKeywords, m_strCurrentWord );
      if( m_currentDotKeyword == nullptr )
      {
         ERROR_MESSAGE( "Unknown attribute: \"" << m_strCurrentWord << "\"" );
         FSM_RETURN();
         return true;
      }
      FSM_TRANSITION( SkipOverFillCharacters, label = '\'=\' recognized' );
      return false;
   }
   if( m_currentChar == ')' )
   {
      ERROR_MESSAGE( "Unexpected end of attribute!" );
      FSM_RETURN();
      return true;
   }
   if( isFillChar( m_currentChar ) )
   {
      FSM_TRANSITION( ReadEqualChar );
      return false;
   }
   ERROR_MESSAGE( "Wrong character \"" << m_currentChar <<
                  "\" in attribute found, expecting \"=\"!");
   FSM_RETURN();
   return true;
}

/*!----------------------------------------------------------------------------
*/
bool AttributeReader::SkipOverFillCharacters( EVENT_T ev )
{
   if( isFillChar( m_currentChar ) )
   {
      FSM_TRANSITION( SkipOverFillCharacters );
      return false;
   }

   m_value.clear();
   if( m_currentChar == '\'' )
   {
      m_value += '"';
      m_inQuotes = true;
   }
   else
   {
      if( m_currentChar == ')' )
      {
         ERROR_MESSAGE( "Unexpected end of attribute!" );
         FSM_RETURN( label='Error' );
         return true;
      }
      m_value += m_currentChar;
      m_inQuotes = false;
   }
   FSM_TRANSITION( ReadValue );
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool AttributeReader::ReadValue( EVENT_T ev )
{
   assert( ev == CHAR );
   if( !m_inQuotes && m_currentChar == ',' )
   {
      FSM_TRANSITION( ReadDotKeyword, label='\',\' recognized' );
      return false;
   }
   if( m_currentChar == '\'' && m_value.back() != '\\' )
   {
      if( m_value.back() == '"' )
      {
         m_value.pop_back();
         m_inQuotes = true;
      }
      else
      {
         m_value += '"';
         m_inQuotes = false;
      }
   }
   else if( m_inQuotes || isKeywordChar( m_currentChar ))
   {
      m_value += m_currentChar;
   }
   if( !m_inQuotes )
   {
      if( m_currentChar == ',' )
         FSM_TRANSITION( ReadDotKeyword );
      else if( m_currentChar == ')' )
         FSM_RETURN( label='End attribute list' );
      return false;
   }
   FSM_TRANSITION( ReadValue );
   return false;
}

/*!----------------------------------------------------------------------------
*/
void AttributeReader::ReadValueExit( void )
{
   assert( dynamic_cast<ATTR_LIST_T*>( m_pAttrList ) != nullptr );

   std::string* pValue = new std::string;
   for( auto& c : m_value )
   {
      if( c != 'n' && pValue->back() == '\\' )
         pValue->pop_back();
      pValue->push_back( c );
   }

   ATTR_T* pAttr = new ATTR_T( *m_currentDotKeyword, pValue );
#ifdef _DEBUG_ATTRIBUTE_READER
   std::cerr << YELLOW "ATTRIBUT: " << std::get<0>(pAttr->first) << " = "
             << *pAttr->second << NORMAL << std::endl;
#endif
   m_pAttrList->push_back( pAttr );
}

//================================== EOF ======================================
