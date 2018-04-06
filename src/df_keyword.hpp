/*****************************************************************************/
/*                                                                           */
/*!          @brief Base-module for recognizing DOCFSM-keywords              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_keyword.hpp                                                  */
/*! @see     df_keyword.cpp                                                  */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_KEYWORD_HPP
#define _DF_KEYWORD_HPP

#ifndef __DOC_FSM__
 #include <string>
 #include <stack>
#endif

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////

class Keyword
{
   friend class SourceBrowser;
   std::string m_oCurrentWord;
   char        m_lastChar;

public:
   Keyword( void );
   virtual ~Keyword( void ) {}

   char getLastChar( void ) const { return m_lastChar; }

   virtual void onRecognized( std::string ) = 0;
   virtual void onChar( char ) = 0;

   static bool isKeywordChar( const char c );
   static bool isFillChar( const char c );

protected:
   bool addIfBelongsToKeyword( const char c );

private:
   void _onChar( char );
};

///////////////////////////////////////////////////////////////////////////////

class SourceBrowser
{
   std::string&         m_rsSource;
   std::stack<Keyword*> m_stack;

public:
   SourceBrowser( std::string& rsSource );

   void set( Keyword* pInterpreter );
   void back( void );
   bool browse( void );
   std::string& getSource( void ) { return m_rsSource; }
};

///////////////////////////////////////////////////////////////////////////////

class KeywordInterpreter: public Keyword
{
   SourceBrowser& m_rSourceBrowser;

public:
   KeywordInterpreter( SourceBrowser& mrSouceBrowser )
      : m_rSourceBrowser( mrSouceBrowser )
   {}

   bool browse( void );
   SourceBrowser& getSourceBrowser( void ) { return m_rSourceBrowser; }

protected:
   void onChar( char ) override;
};

} // End namespace DocFsm
#endif //ifndef _DF_KEYWORD_HPP
//================================== EOF ======================================
