/*****************************************************************************/
/*                                                                           */
/*!              @brief Sub-FSM for reading DOT-attributes                   */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_attribute_reader.hpp                                         */
/*! @see     df_attribute_reader.cpp                                         */
/*! @author  Ulrich Becker                                                   */
/*! @date    02.01.2018                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_ATTRIBUTE_READER_HPP
#define _DF_ATTRIBUTE_READER_HPP

#ifndef __DOCFSM__ // Accelerates DocFsm
 #include "df_keyword.hpp"
 #include "df_dotgenerator.hpp"
#endif

#ifdef _DEBUG_ATTRIBUTE_READER_FSM
   #define _DEBUG_FSM_IMPLEMENTER
#endif
#include "fsm_class_implementer.hpp"

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
/*!
 * @dotfile df_attribute_reader.gv
 */
class AttributeReader: public Keyword
{
   enum EVENT_T
   {
      CHAR,
      WORD
   };

   FSM_CLASS_IMPLEMENT( AttributeReader )

   ATTR_LIST_T*                        m_pAttrList;
   SourceBrowser*                      m_poSourceBrowser;
   const DotKeywords::DOT_ATTR_LIST_T* m_pDotKeywords;
   const DotKeywords::DOT_ATTR_ITEM_T* m_currentDotKeyword;
   char                                m_currentChar;
   std::string                         m_strCurrentWord;
   std::string                         m_value;
   bool                                m_inQuotes;

public:

   AttributeReader( void );

   void start( SourceBrowser& roSorceBrowser, 
               const DotKeywords::DOT_ATTR_LIST_T& roDotKeywords,
               ATTR_LIST_T& rAttrList );

private:

   FSM_DECLARE_STATE( ReadDotKeyword, color=red )
   FSM_DECLARE_STATE( ReadEqualChar, color=yellow )
   FSM_DECLARE_STATE( SkipOverFillCharacters, color=green )
   FSM_DECLARE_STATE_X( ReadValue, color=blue )

   void onRecognized( std::string ) override;
   void onChar( char ) override;
   void fsmReturn( void );
};

} // End namespace DocFsm
#endif // ifndef _DF_ATTRIBUTE_READER_HPP
//================================== EOF ======================================

