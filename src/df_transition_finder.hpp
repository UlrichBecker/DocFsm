/*****************************************************************************/
/*                                                                           */
/*!  @brief Module searches and and collects the ESM-transitions within      */
/*!              the DO-function                                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_transition_finder.hpp                                        */
/*! @see     df_transition_finder.cpp                                        */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_TRANSITION_FINDER_HPP
#define _DF_TRANSITION_FINDER_HPP

#ifndef __DOCFSM__ // Accelerates DocFsm
  #include "df_dotgenerator.hpp"
  #include "df_state_collector.hpp"
  #include "df_attribute_reader.hpp"
#endif

#ifdef FSM_DECLARE_STATE
 #undef FSM_DECLARE_STATE
#endif
#define FSM_DECLARE_STATE( stateName, attr... ) stateName

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
class TransitionFinder: public KeywordInterpreter
{
   enum STATE_T
   {
      __NO_STATE,
      FSM_DECLARE_STATE( OUTSIDE_STATE, color=blue ),
      FSM_DECLARE_STATE( INSIDE_ENUM, color=green ),
      FSM_DECLARE_STATE( STATE_BEGIN, color=red ),
      FSM_DECLARE_STATE( INSIDE_STATE, color=magenta ),
      FSM_DECLARE_STATE( TRANSITION_KEYWORD, color=magenta ),
      FSM_DECLARE_STATE( TRANSITION_ARGUNENTS, color=magenta ),
      FSM_DECLARE_STATE( READ_ATTRIBUTES )
   };

   enum EVENT_T
   {
      CHAR,
      WORD
   };

   class OptionNoMerge: public CLOP::OPTION_V
   {
      bool              m_do;
   public:
      OptionNoMerge( void );
      int onGiven( CLOP::PARSER* poParser ) override;
      bool operator()( void ) const { return m_do; }
   };

   StateCollector&     m_rStates;
   std::string         m_sLastWord;
   char                m_currentEffectivChar;
   char                m_lastEffectiveChar;
   int                 m_braceCount;
   int                 m_exitCount;
   STATE_T             m_currentState;
   STATE_T             m_newState;
   KeywordPool::TYPE_T m_transitionType;
   StateGraph*         m_pStateGraph;
   TransitionGraph*    m_pCurrentTransition;
   OptionNoMerge       m_oOptionNoMerge;

public:
   TransitionFinder( StateCollector& rStates, CommandlineParser& );

   virtual ~TransitionFinder( void );

   bool find( void )
   {
      m_exitCount = -1;
      return browse();
   }

   bool operator()( void )
   {
      return find();
   }

protected:
   bool isThisCharActual( const char );
   void onChar( char ) override;
   void onRecognized( std::string ) override;

private:
   void fsmStep( EVENT_T );
   bool handleBraceCount( void );
   static const ATTR_T* findAttribute( const TransitionGraph&, const DotKeywords::ID_T );
   void mergeAttributes( TransitionGraph& );
   bool addTransition( void );
   void startAttributeReaderTransition( ATTR_LIST_T& rAttrList );
   bool generateExitState( void );
#ifdef _DEBUG_TRANSITION_FINDER_FSM
   const char* state2str( STATE_T );
#endif
};

} // End namespace DocFsm
#endif // ifndef _DF_TRANSITION_FINDER_HPP
//================================== EOF ======================================
