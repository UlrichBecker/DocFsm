/*****************************************************************************/
/*                                                                           */
/*!      @brief Module recognized and collects the state-keywords (macros)   */
/*!             in the C/C++ source-code.                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_state_collector.cpp                                          */
/*! @see     df_state_collector.hpp                                          */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_docfsm.hpp"
#include "df_state_collector.hpp"

using namespace DocFsm;

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::GROUP::GROUP( std::string* pName )
   :m_pName( pName )
{
}

/*!----------------------------------------------------------------------------
*/
StateCollector::GROUP::~GROUP( void )
{
   for( auto& pState : m_vpStates )
      delete pState;
}

/*!----------------------------------------------------------------------------
*/
bool StateCollector::GROUP::haveName( void )
{
   if( m_pName == nullptr )
      return false;

   return !m_pName->empty();
}

/*!----------------------------------------------------------------------------
*/
bool StateCollector::GROUP::doesNotBelongToMe( std::string* pName )
{
   if( pName == m_pName )
      return false;

   if( (pName != nullptr) && (m_pName != nullptr) )
      return *pName != *m_pName;

   return true;
}

/*!----------------------------------------------------------------------------
*/
bool StateCollector::GROUP::doesContaining( StateGraph* pState )
{
   for( auto& i_pState : m_vpStates )
      if( pState == i_pState )
         return true;
   return false;
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::GROUP::addStatePtrIfNotAlreadyContaining( StateGraph* pState )
{
   for( auto& i_pState : m_vpStates )
      if( pState == i_pState )
         return;
   m_vpStates.push_back( pState );
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::GROUP::printStates( std::ostream& rOut, int tabs )
{
   for( auto& pStates : m_vpStates )
      pStates->printState( rOut, tabs );
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::GROUP::printTransitions( std::ostream& rOut, int tabs )
{
   for( auto& pStates : m_vpStates )
      pStates->printTransitions( rOut, tabs );
}


///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::FSM::FSM( void )
{
   m_vpGroups.push_back( new GROUP );
}

/*!----------------------------------------------------------------------------
*/
StateCollector::FSM::~FSM( void )
{
   for( auto& pGroup : m_vpGroups )
      delete pGroup;
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::FSM::splitInGroups( void )
{
   if( m_vpGroups.empty() )
      return 0;

   GROUP* pBaseGroup = m_vpGroups.back();
   assert( pBaseGroup != nullptr );
   if( pBaseGroup->m_vpStates.empty() )
      return 0;

   assert( pBaseGroup->m_pName == nullptr );
   pBaseGroup->m_pName = pBaseGroup->m_vpStates.back()->findGroupName();


   for( auto it_pState = pBaseGroup->m_vpStates.begin(); 
             it_pState != pBaseGroup->m_vpStates.end();  )
   {
      std::string* pGroupName = (*it_pState)->findGroupName();
      bool doesNotFit = pBaseGroup->doesNotBelongToMe( pGroupName );
      if( !doesNotFit )
      {
         it_pState++;
         continue;
      }
      for( auto& pGroup : m_vpGroups )
      {
         if( pGroup == pBaseGroup )
            continue;
         if( pGroup->doesNotBelongToMe( pGroupName ) )
            continue;

         pGroup->m_vpStates.push_back( *it_pState );
         it_pState = pBaseGroup->m_vpStates.erase( it_pState );
         doesNotFit = false;
      }
      if( !doesNotFit )
         continue;
      GROUP* pGroup = new GROUP( pGroupName );
      pGroup->m_vpStates.push_back( *it_pState );
      m_vpGroups.push_back( pGroup );
      it_pState = pBaseGroup->m_vpStates.erase( it_pState );
   }

   return 0;
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::FSM::print( std::ostream& rOut, 
                                 const std::string& clusterName, int tabs )
{
   int groupNum = 0;
   for( auto& pGroup : m_vpGroups )
   {
      if( pGroup->haveName() )
      {
         printTabs( rOut, tabs );
         rOut << clusterName << groupNum << " {\n";
         tabs++;
         printTabs( rOut, tabs );
         rOut << c_strLabel << pGroup->getName() << "\n";
      }
      pGroup->printStates( rOut, tabs );
      if( pGroup->haveName() )
      {
         tabs--;
         printTabs( rOut, tabs );
         rOut << "}\n";
         groupNum++;
      }
   }

   for( auto& pGroup : m_vpGroups )
      pGroup->printTransitions( rOut, tabs );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::MODULE::MODULE( const std::string& name )
   : m_name( name )
{
   m_vpFsm.push_back( new FSM );
}

/*!----------------------------------------------------------------------------
*/
StateCollector::MODULE::~MODULE( void )
{
   for( auto& pFsm : m_vpFsm )
      delete pFsm;

   for( auto& pAttribute : m_vpAttributes )
      delete pAttribute;
}

/*!----------------------------------------------------------------------------
*/
#if 0 //RFU
int StateCollector::MODULE::isATarget( const StateGraph* pStateGraph )
{
   FSM* pOriginFsm = m_vpFsm.back();
   for( auto& pState : pOriginFsm->getStateList() )
   {
      if( pState == pStateGraph )
         continue;
      for( auto& pTransition : pState->m_vpTransitions )
      {
         if( pTransition->m_pTargetState != pStateGraph )
            continue;
         if( pState->m_fsmNumber != 0 )
            return pState->m_fsmNumber;
      }
   }
   return 0;
}
#endif
/*!----------------------------------------------------------------------------
*/
int StateCollector::MODULE::splitInClusters( void )
{
   FSM* pOriginFsm = m_vpFsm.back();
   int maxFsmNumber = 0;
   bool inFsm;
   do
   {
      maxFsmNumber++;
      for( auto& i : pOriginFsm->getStateList() )
      {
         if( i->getFsmNumber() != 0 )
            continue;
         i->setFsm( maxFsmNumber );
         break;
      }
      inFsm = true;
      for( auto& i : pOriginFsm->getStateList() )
      {
         i->obtainFsmNumberIfEqual( maxFsmNumber );
         if( i->getFsmNumber() == 0 )
            inFsm = false;
      }
   }
   while( !inFsm );

   for( int fsm = 2; fsm <= maxFsmNumber; fsm++ )
   {
      FSM* pNewFsm = nullptr;
      for( auto it = pOriginFsm->getStateList().begin(); it != pOriginFsm->getStateList().end(); )
      {
         if( ((*it)->getFsmNumber() == fsm) )
         {
            if( pNewFsm == nullptr )
            {
               pNewFsm = new FSM;
               m_vpFsm.push_back( pNewFsm );
            }
            pNewFsm->getStateList().push_back( *it );
            it = pOriginFsm->getStateList().erase( it );
         }
         else
            it++;
      }
   }

   return maxFsmNumber;
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::MODULE::splitInGroups( void )
{
   for( auto& pFsm : m_vpFsm )
       pFsm->splitInGroups();
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetGraphAttributes::SetGraphAttributes( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg   = REQUIRED_ARG;
   m_shortOpt = 'G';
   m_longOpt  = "graph";
   m_helpText = "Set global graph attributes.\n"
                "NOTE: For each additional attribute use a separate option-label.\n"
                "E.g.: -G rankdir=LR -G rotate=90 "
                "-G \"label=\\\"My very nice finite state-machine!\\\"\" ...";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetGraphAttributes::onGiven( CLOP::PARSER* poParser )
{
   if( m_pParent->addGraphAttribute( poParser->getOptArg() ))
   {
      ::exit( EXIT_FAILURE );
      return -1;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetNodeAttributes::SetNodeAttributes( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg   = REQUIRED_ARG;
   m_shortOpt = 'N';
   m_longOpt  = "node";
   m_helpText = "Set global default node (state) attributes.\n"
                "NOTE: For each additional attribute use a separate option-label.\n"
                "E.g.: -N color=red  -N shape=house ...\n"
                "The default attribute \"shape=Mrecord\" will overwritten in any cases.";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetNodeAttributes::onGiven( CLOP::PARSER* poParser )
{
   if( m_pParent->addNodeAttribute( poParser->getOptArg() ))
   {
      ::exit( EXIT_FAILURE );
      return -1;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetEdgeAttributes::SetEdgeAttributes( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg   = REQUIRED_ARG;
   m_shortOpt = 'E';
   m_longOpt  = "edge";
   m_helpText = "Set global default edge (transition) attributes.\n"
                "NOTE: For each additional attribute use a separate option-label.\n"
                "E.g.: -E color=pink -E style=dashed ...";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetEdgeAttributes::onGiven( CLOP::PARSER* poParser )
{
   if( m_pParent->addEdgeAttribute( poParser->getOptArg() ))
   {
      ::exit( EXIT_FAILURE );
      return -1;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetSingleGraph::SetSingleGraph( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg    = NO_ARG;
   m_shortOpt  = 's';
   m_longOpt   = "single";
   m_helpText  = "If given, than in the case of more then one source-file no\n"
                 "additional clusters per source-file will build.\n"
                 "The MODULE's of all source-files will put together in a single graph.\n"
                 "E.g.: -s myFsmPart1.cpp myFsmPart2.cpp";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetSingleGraph::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_isSingle = true;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetNoStateGroups::SetNoStateGroups( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg    = NO_ARG;
   m_shortOpt  = 'u';
   m_longOpt   = "nostategroups";
   m_helpText  = "Despite the DocFsm attribute \"GROUP\" is set or not,\n"
                 "when this option is set no no state-groups will build.";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetNoStateGroups::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_noStateGroups = true;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetNoFsmGroups::SetNoFsmGroups( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg    = NO_ARG;
   m_shortOpt  = 'f';
   m_longOpt   = "nofsmgroups";
   m_helpText  = "No split in FSM-groups, when unconnected states detected.";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetNoFsmGroups::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_noFsmGroups = true;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetNoTransitions::SetNoTransitions( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg    = NO_ARG;
   m_shortOpt  = 'T';
   m_longOpt   = "notransitions";
   m_helpText  = "Print states only, no transitions.";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetNoTransitions::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_noTransitions = true;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
StateCollector::SetNoTransitionLabel::SetNoTransitionLabel( StateCollector* pParent )
   :Option( pParent )
{
   m_hasArg    = NO_ARG;
   m_shortOpt  = 'O';
   m_longOpt   = "noTrLabel";
   m_helpText  = "Omit printing of transition text-labels.";
}

/*!----------------------------------------------------------------------------
*/
int StateCollector::SetNoTransitionLabel::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_noTransitionLabels = true;
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const std::string StateCollector::c_strLabel = "label = ";

/*!----------------------------------------------------------------------------
*/
StateCollector::StateCollector( SourceBrowser& rSourceBrowser, 
                                KeywordPool& rKeywords,
                                CommandlineParser& rCommandLine
                              )
   :KeywordInterpreter( rSourceBrowser )
   ,m_setGraphAttributes( this )
   ,m_setNodeAttributes( this )
   ,m_setEdgeAttributes( this )
   ,m_setSingleGraph( this )
   ,m_setNoStateGroups( this )
   ,m_setNoFsmGroups( this )
   ,m_setNoTransitions( this )
   ,m_setNoTransitionLabel( this )
   ,m_rKeywords( rKeywords )
   ,m_parseArgument( false )
   ,m_isSingle( false )
   ,m_noStateGroups( false )
   ,m_noFsmGroups( false )
   ,m_noTransitions( false )
   ,m_noTransitionLabels( false )
   ,m_entryCount( 0 )
{
   rCommandLine( m_setGraphAttributes )
               ( m_setNodeAttributes )
               ( m_setEdgeAttributes )
               ( m_setSingleGraph )
               ( m_setNoStateGroups )
               ( m_setNoFsmGroups )
               ( m_setNoTransitions )
               ( m_setNoTransitionLabel );
   m_pLabelAttribute = DotKeywords::findNodeWord( "label" );
   assert( m_pLabelAttribute != nullptr );
}

/*!----------------------------------------------------------------------------
*/
StateCollector::~StateCollector( void )
{
   for( auto& pGerphAttribute : m_vpGraphAttributes )
      delete pGerphAttribute;

   for( auto& pNodeAttribute : m_vpNodeAttributes )
      delete pNodeAttribute;

   for( auto& pEdgeAttribure : m_vpEdgeAttributes )
      delete pEdgeAttribure;

   for( auto& pModule : m_vpModules )
      delete pModule;
}

/*!----------------------------------------------------------------------------
*/
bool StateCollector::collect( const std::string& rName )
{
   m_entryCount = 0;
   m_vpModules.push_back( new MODULE( rName ) );

   if( browse(COLLECT_TRANSITIONS) )
      return true;

   if( browse(COLLECT_DECLARATIONS) )
      return true;

   if( get().empty() )
   {
      delete m_vpModules.back();
      m_vpModules.pop_back();
      std::stringbuf str;
      std::ostream   outStream(&str);
      outStream << "\"";
      m_rKeywords.listTransitionKeywords( outStream );
      outStream << "\" and/or \"";
      m_rKeywords.listDeclareKeywords( outStream );
      outStream << "\"";
      WARNING_MESSAGE( "No keywords " << str.str() << " "
                       "in file: \"" << rName << "\" found!" );
      return false;
   }

   if( browse(COLLECT_INITIALIZATIONS))
      return true;

   return false;
}

/*!----------------------------------------------------------------------------
 * TODO
*/
void StateCollector::onRecognized( std::string word )
{
   if( m_parseArgument )
   {
      StateGraph* poTargetStade = find( word );

      if( (m_toCollect == COLLECT_DECLARATIONS) || 
          (m_toCollect == COLLECT_TRANSITIONS) )
      {
         m_parseArgument = false;
         if( poTargetStade == nullptr )
            poTargetStade = add( word );
         if( m_toCollect == COLLECT_DECLARATIONS )
            readStateAttributes( poTargetStade );
         return;
      }

      if( poTargetStade == nullptr )
         return;

      if( m_toCollect == COLLECT_INITIALIZATIONS )
      {
         m_parseArgument = false;
         prepareEntryPoint( poTargetStade );
      }
      return;
   }

   switch( m_toCollect )
   {
      case COLLECT_TRANSITIONS:
      {
         if( m_rKeywords.isOneOfTransitionKeyWords( word ))
            m_parseArgument = true;
         return;
      }
      case COLLECT_INITIALIZATIONS:
      {
         if( m_rKeywords.isOneOfInitialKeyWords( word ))
            m_parseArgument = true;
         return;
      }
      case COLLECT_DECLARATIONS:
      {
         if( m_rKeywords.isOneOfDeclareKeyWords( word ))
            m_parseArgument = true;
         return;
      }
      default: assert( false ); break;
   }
}

/*!----------------------------------------------------------------------------
 * [shape=circle, style=filled, color=black, label=""]
*/
bool StateCollector::prepareEntryPoint( StateGraph* poTargetStade )
{
   StateGraph* poStade = addIfNotAlreadyDone( "entry" + std::to_string( m_entryCount ));
   if( poStade == nullptr )
      return true;
   m_entryCount++;
   TransitionGraph* pTransitionGraph = new TransitionGraph( poTargetStade );
   poStade->addTransition( pTransitionGraph );

#ifdef CONFIG_POINT_AS_ENTRY_EXIT_STATE
   poStade->addAttribute( "shape", "point" );
#else
   poStade->addAttribute( "shape", "circle" );
   poStade->addAttribute( "style", "filled" );
   poStade->addAttribute( "color", "black" );
   poStade->addAttribute( "label", "\"\"" );
#endif
   m_oAttributeReader.start( getSourceBrowser(),
                             DotKeywords::c_edgeAttributes,
                             pTransitionGraph->getAttrList() );
   return false;
}

/*!----------------------------------------------------------------------------
*/
bool StateCollector::readStateAttributes( StateGraph* poTargetStade )
{
   m_oAttributeReader.start( getSourceBrowser(),
                             DotKeywords::c_nodeAttributes,
                             poTargetStade->getAttrList() );
   return false;
}

/*!----------------------------------------------------------------------------
*/
StateGraph* StateCollector::find( const std::string& name )
{
   for( auto& pModul: m_vpModules )
      for( auto& pFsm: pModul->m_vpFsm )
         for( auto& pGroup: pFsm->m_vpGroups )
            for( auto& pState: pGroup->m_vpStates )
               if( pState->getName() == name )
                  return pState;

   return nullptr;
}

/*!----------------------------------------------------------------------------
*/
bool StateCollector::addGlobalAttribute( const DotKeywords::DOT_ATTR_LIST_T& rDotAttrList,
                                         ATTR_LIST_T& rAttr,
                                         const std::string& rStr )
{
   const DotKeywords::DOT_ATTR_ITEM_T* pAttrItem;
   std::string temp;
   bool isArgument = false;

   for( auto& c : rStr )
   {
      if( (c == '=') && !isArgument )
      {
         isArgument = true;
         if( temp.empty() )
         {
            ERROR_MESSAGE( "Missing DOT-keyword" );
            return true;
         }
         pAttrItem = DotKeywords::find( rDotAttrList, temp );
         if( pAttrItem == nullptr )
         {
            ERROR_MESSAGE( "Unknown DOT-keyword \"" << temp << "\"" );
            return true;
         }
         temp.clear();
         continue;
      }
      temp += c;
   }
   if( !isArgument || temp.empty() )
   {
      ERROR_MESSAGE( "Missing attribute argument!" );
      return true;
   }
   assert( pAttrItem != nullptr );
   rAttr.push_back( new ATTR_T( *pAttrItem, new std::string( temp )));
   return false;
}

/*!----------------------------------------------------------------------------
*/
StateGraph* StateCollector::addIfNotAlreadyDone( const std::string& rWord )
{
   if( find( rWord ) != nullptr )
      return nullptr;

   return add( rWord );
}

/*!----------------------------------------------------------------------------
*/
StateGraph* StateCollector::add( const std::string& rWord )
{
   StateGraph* poStade = new StateGraph( this, rWord );
   if( poStade != nullptr );
      get().push_back( poStade );
   return poStade;
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::setGlobalAttributesIfEmpty( void )
{
   if( m_vpNodeAttributes.empty() )
   {
      const DotKeywords::DOT_ATTR_ITEM_T* pAttr = DotKeywords::findNodeWord( "shape" );
      assert( pAttr != nullptr );
      m_vpNodeAttributes.push_back( new ATTR_T( *pAttr, new std::string("Mrecord") ) );
   }
}

/*!----------------------------------------------------------------------------
*/
std::string StateCollector::baseFileName( const std::string& rStr )
{
#ifdef _WIN32
   const char sep = '\\';
#else
   const char sep = '/';
#endif

   std::size_t i = rStr.rfind( sep, rStr.length() );
   if( i != std::string::npos )
      return( rStr.substr( i+1, rStr.length()-i ));

   return( rStr );
}

/*!----------------------------------------------------------------------------
*/
std::string StateCollector::stripFileName( const std::string& rStr )
{
   std::string name = baseFileName( rStr );
   std::size_t i = name.find( '.' );
   if( i != std::string::npos )
      return name.substr( 0, i );
   return name;
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::splitInClusters( void )
{
   int moduleClusterNumber = 0;
   for( auto& pModule : m_vpModules )
   {
      if( !m_noFsmGroups )
         pModule->splitInClusters();
      for( auto& pFsm : pModule->m_vpFsm )
         for( auto& vpStates : pFsm->getStateList() )
             vpStates->addClusterNumber( moduleClusterNumber );
      moduleClusterNumber++;
   }
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::splitInGroups( void )
{
   if( m_noStateGroups )
      return;

   for( auto& pModule : m_vpModules )
      pModule->splitInGroups();
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::printTabs( std::ostream& rOut, int tabs )
{
   for( int i = 0; i < tabs; i++ )
      rOut << '\t';
}

/*!----------------------------------------------------------------------------
*/
void StateCollector::print( std::ostream& rOut )
{
   if( m_vpModules.size() == 0 )
      return;

   setGlobalAttributesIfEmpty();
   rOut << "digraph \"generated by DocFsm\" {\n";
   if( !m_vpGraphAttributes.empty() )
   {
      rOut << "\tgraph";
      StateGraph::printAttr( rOut, m_vpGraphAttributes );
      rOut << ";\n";
   }
   if( !m_vpNodeAttributes.empty() )
   {
      rOut << "\tnode";
      StateGraph::printAttr( rOut, m_vpNodeAttributes );
      rOut << ";\n";
   }
   if( !m_vpEdgeAttributes.empty() )
   {
      rOut << "\tedge";
      StateGraph::printAttr( rOut, m_vpEdgeAttributes );
      rOut << ";\n";
   }

   bool useSubgraphs = !m_isSingle && (m_vpModules.size() > 1);
   int moduleClusterNumber = 0;
   for( auto& pModule : m_vpModules )
   {
      const std::string c_strSubgraph( "subgraph cluster_" );

      if( useSubgraphs )
      {
         rOut << "\t" << c_strSubgraph << moduleClusterNumber << " {\n";
         rOut << "\t\t" << c_strLabel << "\"" << pModule->m_name << "\";\n";
      }

      int fsmClusterNumber = 0;
      for( auto& pFsm : pModule->m_vpFsm )
      {
         int tabs = useSubgraphs? 2 : 1;
         std::string clusterName = c_strSubgraph;

         if( pModule->m_vpFsm.size() > 1 )
         {
            printTabs( rOut, tabs );
            clusterName += std::to_string( moduleClusterNumber );
            clusterName += '_';
            clusterName += std::to_string( fsmClusterNumber );
            rOut << clusterName << " {\n";
            tabs++;
            printTabs( rOut, tabs );
            rOut << c_strLabel << "\"FSM: " << fsmClusterNumber << "\";\n";
         }

         pFsm->print( rOut, clusterName, tabs );

         if( pModule->m_vpFsm.size() > 1 )
         {
            tabs--;
            printTabs( rOut, tabs );
            rOut << "}\n";
         }
         fsmClusterNumber++;
      }

      if( useSubgraphs )
         rOut << "\t}\n";

      moduleClusterNumber++;
   }
   rOut << "}" << std::endl;
}

//================================== EOF ======================================
