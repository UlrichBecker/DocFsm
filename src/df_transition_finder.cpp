/*****************************************************************************/
/*                                                                           */
/*!  @brief Module searches and and collects the ESM-transitions within      */
/*!              the DO-function                                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_transition_finder.cpp                                        */
/*! @see     df_transition_finder.hpp                                        */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_docfsm.hpp"
#include "df_transition_finder.hpp"

using namespace DocFsm;

#ifdef FSM_TRANSITION
   #undef FSM_TRANSITION
#endif

#ifdef _DEBUG_TRANSITION_FINDER_FSM
   #define FSM_TRANSITION( _newState, attr... )                            \
   do                                                                      \
   {                                                                       \
      std::cerr << state2str( m_currentState ) << " -> "                   \
                << state2str( _newState ) << std::endl;                    \
      m_newState = _newState;                                              \
   }                                                                       \
   while( false )

   #define FSM_TRANSITION_SELF( attr... )                                  \
   do                                                                      \
   {                                                                       \
      std::cerr << state2str( m_currentState ) << " -> "                   \
                << state2str( m_currentState ) << std::endl;               \
   }                                                                       \
   while( false )

#else
   #define FSM_TRANSITION( _newState, attr... )  m_newState = _newState
   #define FSM_TRANSITION_SELF( attr... )
#endif


#ifdef FSM_INIT_FSM
  #undef FSM_INIT_FSM
#endif
#define FSM_INIT_FSM( startState, attr... )                                \
       m_currentState( startState ),                                       \
       m_newState( startState )

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
TransitionFinder::OptionNoMerge::OptionNoMerge( void )
   :m_do( false )
{
   m_hasArg   = NO_ARG;
   m_shortOpt = 'n';
   m_longOpt  = "nomerge";
   m_helpText = "Do not merge transitions with the same target in the same state.";
}

/*!----------------------------------------------------------------------------
*/
int TransitionFinder::OptionNoMerge::onGiven( CLOP::PARSER* poParser )
{
   m_do = true;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
TransitionFinder::TransitionFinder( StateCollector& rStates,
                                    CommandlineParser& rCommandlineparser )
   :KeywordInterpreter( rStates.getSourceBrowser() )
   ,m_rStates( rStates )
   ,m_currentEffectivChar( '\0' )
   ,m_lastEffectiveChar( '\0' )
   ,m_braceCount( 0 )
   ,m_exitCount( -1 )
   ,FSM_INIT_FSM( OUTSIDE_STATE, color=blue, label='Start' )
   ,m_pStateGraph( nullptr )
   ,m_pCurrentTransition( nullptr )
{
   rCommandlineparser( m_oOptionNoMerge );
}

/*!----------------------------------------------------------------------------
*/
TransitionFinder::~TransitionFinder( void )
{}

/*!----------------------------------------------------------------------------
*/
#ifdef _DEBUG_TRANSITION_FINDER_FSM
const char* TransitionFinder::state2str( STATE_T state )
{
   #define _CASE_ITEM( _state ) case _state: return #_state;
   switch( state )
   {
      _CASE_ITEM( OUTSIDE_STATE )
      _CASE_ITEM( INSIDE_ENUM )
      _CASE_ITEM( STATE_BEGIN )
      _CASE_ITEM( INSIDE_STATE )
      _CASE_ITEM( TRANSITION_KEYWORD )
      _CASE_ITEM( TRANSITION_ARGUNENTS )
      _CASE_ITEM( READ_ATTRIBUTES )
      default: break;
   }
   return "unknown";
   #undef _CASE_ITEM
}
#endif // ifdef _DEBUG_TRANSITION_FINDER_FSM

/*!----------------------------------------------------------------------------
*/
void TransitionFinder::onChar( char c )
{
   if( addIfBelongsToKeyword( c ) || isFillChar( c ) )
   {
      m_lastEffectiveChar = '\0';
      return;
   }

   m_currentEffectivChar = c;
   if( m_lastEffectiveChar != '\'' )
      fsmStep( CHAR );
   m_lastEffectiveChar = c;
}

/*!----------------------------------------------------------------------------
*/
void TransitionFinder::onRecognized( std::string word )
{
   m_sLastWord = word;
   fsmStep( WORD );
}

/*!----------------------------------------------------------------------------
*/
inline bool TransitionFinder::isThisCharActual( const char c )
{
   return (m_currentEffectivChar == c);
}

/*!----------------------------------------------------------------------------
*/
bool TransitionFinder::handleBraceCount( void )
{
   if( isThisCharActual('{') )
   {
      m_braceCount++;
      return false;
   }
   if( isThisCharActual('}') )
   {
      m_braceCount--;
      if( m_braceCount == 0 )
         return true;
   }
   if( m_braceCount < 0 )
   {
      if( m_pStateGraph != nullptr )
      {
         assert( dynamic_cast<StateGraph*>(m_pStateGraph) != nullptr);
         ERROR_MESSAGE( "Missing \'{\' whithin state \""
                        << m_pStateGraph->getName()
                        << "\nLastchar: " << m_lastEffectiveChar );
      }
#if 1
      else
      {
         ERROR_MESSAGE( "Missing \'{\' whithin emum definition \""
                         << "\nLastchar: " << m_lastEffectiveChar );
      }
#endif
   }
   return false;
}

/*!----------------------------------------------------------------------------
*/
const ATTR_T* TransitionFinder::findAttribute(  TransitionGraph& rTransition,
                                         const DotKeywords::ID_T id )
{
   for( const auto& pAttr : rTransition.getAttrList() )
   {
      if( DotKeywords::getId(pAttr->first) == id )
         return pAttr;
   }
   return nullptr;
}

/*!----------------------------------------------------------------------------
*/
void TransitionFinder::mergeAttributes( TransitionGraph& rPresent )
{
   assert( dynamic_cast<TransitionGraph*>(m_pCurrentTransition) != nullptr );

   const ATTR_T* pCurrentAttr = findAttribute( *m_pCurrentTransition, DotKeywords::LABEL );
   if( pCurrentAttr == nullptr )
       return;
   if( pCurrentAttr->second == nullptr )
       return;
   if( pCurrentAttr->second->empty() )
       return;
   const ATTR_T* pPresentAttr = findAttribute( rPresent, DotKeywords::LABEL );
   if( pPresentAttr != nullptr )
   {
      if( !pPresentAttr->second->empty() )
      {
         pPresentAttr->second->pop_back(); //remove quote (")
         *pPresentAttr->second += "\\n";
         // Append string after beginning quote (")
         *pPresentAttr->second += pCurrentAttr->second->substr( 1 );
      }
      else
         *pPresentAttr->second += *pCurrentAttr->second;
   }
   else
   {
      rPresent.getAttrList().push_back( new ATTR_T( pCurrentAttr->first,
                                             new std::string( *pCurrentAttr->second ) ));
   }
}

/*!----------------------------------------------------------------------------
*/
bool TransitionFinder::addTransition( void )
{
   if( m_pCurrentTransition == nullptr )
       return false;

   assert( dynamic_cast<StateGraph*>(m_pStateGraph) != nullptr );
   assert( dynamic_cast<TransitionGraph*>(m_pCurrentTransition) != nullptr );

   if( !m_oOptionNoMerge() )
   {
      for( auto& pTransition : m_pStateGraph->getTransitionList() )
      {
         if( pTransition->getTargetState()->getName() != 
             m_pCurrentTransition->getTargetState()->getName() )
            continue;
         mergeAttributes( *pTransition );
         delete m_pCurrentTransition;
         m_pCurrentTransition = nullptr;
         return false;
      }
   }

   m_pStateGraph->addTransition( m_pCurrentTransition );
   m_pCurrentTransition = nullptr;
   return true;
}

/*!----------------------------------------------------------------------------
 * [shape=doublecircle, style=filled, color=black, label=""];
*/
bool TransitionFinder::generateExitState( void )
{
   StateGraph* pStateGraph = m_rStates.addIfNotAlreadyDone( "exit" + 
                                              std::to_string( m_exitCount ) );
   if( pStateGraph == nullptr )
      return false;

#ifdef CONFIG_POINT_AS_ENTRY_EXIT_STATE
   pStateGraph->addAttribute( "shape", "point" );
#else
   pStateGraph->addAttribute( "shape", "doublecircle" );
   pStateGraph->addAttribute( "style", "filled" );
   pStateGraph->addAttribute( "color", "black" );
   pStateGraph->addAttribute( "label", "\"\"" );
#endif
   m_pCurrentTransition = new TransitionGraph( pStateGraph );
   return true;
}

/*!----------------------------------------------------------------------------
*/
inline void TransitionFinder::startAttributeReaderTransition( ATTR_LIST_T& rAttrList )
{
   m_rStates.getAttributeReader().start( getSourceBrowser(),
                                         DotKeywords::c_edgeAttributes,
                                         rAttrList );
}

/*!----------------------------------------------------------------------------
 * @dotfile df_transition_finder.gv
*/
void TransitionFinder::fsmStep( EVENT_T event )
{
   /*
    * Entry activities
    */
   if( m_newState != m_currentState )
   {
      switch( m_newState )
      {
         case INSIDE_STATE:
         {
            addTransition();
            break;
         }
         case OUTSIDE_STATE:
         {
            m_pStateGraph = nullptr;
            m_pCurrentTransition = nullptr;
            m_exitCount++;
            break;
         }
         default: break;
      }
      m_currentState = m_newState;
   }

   /*
    * Do activities
    */
   switch( m_currentState )
   {
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case OUTSIDE_STATE:
      {
         if( event == CHAR )
            break;
         if( isThisCharActual('(') )
            break;
         assert( m_pStateGraph == nullptr );
         m_pStateGraph = m_rStates.find( m_sLastWord );
         if( m_pStateGraph != nullptr ) //found!
         {
            FSM_TRANSITION( STATE_BEGIN, label = 'perhaps Do-function\n'
                                                 'of state found',
                                         color = red );
            break;
         }
         if( m_sLastWord == "enum" )
         {
            FSM_TRANSITION( INSIDE_ENUM, label = 'Enum definition found',
                                         color = green );
            break;
         }
#ifdef __DOCFSM__
         FSM_TRANSITION( OUTSIDE_STATE ); //!<@deprecated
#endif
         FSM_TRANSITION_SELF( color = blue ); //!<@todo
         break;
      } // End of case OUTSIDE_STATE
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case INSIDE_ENUM:
      {
         if( event == WORD )
            break;

         if( handleBraceCount() )
         {
            FSM_TRANSITION( OUTSIDE_STATE, label = 'End of enum-definition' );
            break;
         }
         if( m_braceCount < 0 )
            return;

#ifdef __DOCFSM__
         FSM_TRANSITION( INSIDE_ENUM ); //!<@deprecated
#endif
         FSM_TRANSITION_SELF(); //!<@todo
         break;
      } // End of case INSIDE_ENUM
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case STATE_BEGIN:
      {
         if( (event == WORD) || (event == CHAR && isThisCharActual(';')))
         {
            FSM_TRANSITION( OUTSIDE_STATE, label = 'No, wasn\'t the header\n'
                                                   'of a do-function' );
            break;
         }
         FSM_TRANSITION( INSIDE_STATE );
         break;
      }
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case INSIDE_STATE:
      {
         if( event == CHAR )
         {
            if( handleBraceCount() )
            {
               FSM_TRANSITION( OUTSIDE_STATE, label = 'End of do-function' );
               break;
            }
            if( m_braceCount < 0 )
               return;

            break;
         } 
         assert( event == WORD );
         if( m_braceCount == 0 )
            break;

         m_transitionType = m_rStates.getKeywords().determineTransitionType( m_sLastWord );
         if( m_transitionType != KeywordPool::NON )
            FSM_TRANSITION( TRANSITION_KEYWORD, label='FSM-keyword found' );
#ifdef __DOCFSM__
         FSM_TRANSITION( INSIDE_STATE ); //!<@deprecated
#endif
         FSM_TRANSITION_SELF(); //!<@todo
         break;
      } // End of case INSIDE_STATE
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case TRANSITION_KEYWORD:
      {
         if( !isThisCharActual('(') )
         {
            FSM_TRANSITION( INSIDE_STATE );
            break;
         }
         switch( m_transitionType )
         {
            case KeywordPool::NON: { assert(false); break; }
            case KeywordPool::TRANSITION:
            {
               FSM_TRANSITION ( TRANSITION_ARGUNENTS, label='Is transition-keyword' );
               break;
            }
            case KeywordPool::RETURN:
            {
               if( generateExitState() )
               {
                  startAttributeReaderTransition( m_pCurrentTransition->getAttrList() );
                  FSM_TRANSITION( INSIDE_STATE );
               }
                 // FSM_TRANSITION( READ_ATTRIBUTES, label='Is return-keyword' );
               break;
            }
#if 0
            case KeywordPool::CALL:
            {
               //TODO Generate Sub-State!
               break;
            }
#endif
            default: FSM_TRANSITION( INSIDE_STATE ); break; //TODO
         }
         break;
      } // End of case TRANSITION_KEYWORD
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case TRANSITION_ARGUNENTS:
      {
         if( event == CHAR )
         {
            if( isThisCharActual(')') )
            {
               FSM_TRANSITION( INSIDE_STATE, label='End of argument-list\n'
                                                   'reached' );
               break;
            }
            break;
         }
         assert( event == WORD );
         StateGraph* poState = m_rStates.find( m_sLastWord );
         if( poState == nullptr )
         {
#ifdef __DOCFSM__
            FSM_TRANSITION( TRANSITION_ARGUNENTS ); //!<@deprecated
#endif
            FSM_TRANSITION_SELF(); //!<@todo
            break;
         }
         m_pCurrentTransition = new TransitionGraph( poState );
         FSM_TRANSITION( READ_ATTRIBUTES );
         break;
      } // End of case TRANSITION_ARGUNENTS
   //--  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
      case READ_ATTRIBUTES:
      {
         assert( dynamic_cast<TransitionGraph*>( m_pCurrentTransition ) != nullptr );

         if( event != CHAR )
            break;

         if( isThisCharActual(',') )
         {
            startAttributeReaderTransition( m_pCurrentTransition->getAttrList() );
            FSM_TRANSITION( INSIDE_STATE );
            break;
         }

         if( isThisCharActual(')') )
         {
            FSM_TRANSITION( INSIDE_STATE );
            break;
         }
#ifdef __DOCFSM__
         FSM_TRANSITION( READ_ATTRIBUTES ); //!<@deprecated
#endif
         FSM_TRANSITION_SELF(); //!<@todo
         break;
      } // End of case READ_ATTRIBUTES
   }

   if( m_newState == m_currentState )
      return;

   /*
    * Exit activities.
    */
   switch( m_currentState )
   {
      case OUTSIDE_STATE:
      {
         m_braceCount = 0;
         break;
      }
      default: break;
   }
}

//================================== EOF ======================================
