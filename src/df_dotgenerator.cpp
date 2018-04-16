/*****************************************************************************/
/*                                                                           */
/*!      @brief Module creates the DOT-output ready for GRAPHVIZ             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_dotgenerator.cpp                                             */
/*! @see     df_dotgenerator.hpp                                             */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef __DOCFSM__
 #include "df_docfsm.hpp"
 #include "df_state_collector.hpp"
 #include "df_keyword.hpp"
#endif
#include "df_dotgenerator.hpp"

using namespace DocFsm;

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
void ATTR_LIST_T::generateTooltipFromLabel( const DotKeywords::DOT_ATTR_LIST_T& rList )
{
   for( auto& pAttr: *this )
      if( DotKeywords::getId( pAttr->first ) == DotKeywords::TOOLTIP )
         return; // Tooltip already defined in source-code.

   std::string* pLabelText = nullptr;
   for( auto& pAttr: *this )
   {
      if( DotKeywords::getId( pAttr->first ) == DotKeywords::LABEL )
      {
         pLabelText = pAttr->second;
         break;
      }
   }
   if( pLabelText == nullptr || pLabelText->empty() )
      return; // No label preset respectively no label-text.

   const DotKeywords::DOT_ATTR_ITEM_T* pToolTip = DotKeywords::find( rList, "tooltip" );
   assert( pToolTip != nullptr );

   std::string* pToolTipText = new std::string;

   for( auto it = pLabelText->begin(); it != pLabelText->end(); it++ )
   {
      if( *it == 'n' && pToolTipText->back() == '\\' )
      {
         pToolTipText->pop_back();
         pToolTipText->push_back( ' ' );
      }
      else
         pToolTipText->push_back( *it );
   }
   pToolTipText->shrink_to_fit();
   push_back( new ATTR_T( *pToolTip, pToolTipText ) );
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
void StateGraph::addClusterNumber( int n )
{
   bool labelPresent = false;
   for( auto& pAttribute : m_vpAttributes )
   {
      if( &(pAttribute->first) == m_pParent->getLabelAttribute() )
      {
         labelPresent = true;
         break;
      }
   }
   if( !labelPresent )
   {
      m_vpAttributes.push_back( new ATTR_T( *m_pParent->getLabelAttribute(),
                                    new std::string('"' + m_name + '"') ) );
   }
   m_clusterNumber = n;
}

/*!----------------------------------------------------------------------------
*/
void StateGraph::printName( std::ostream& rOut )
{
   rOut << m_name << '_' <<  m_clusterNumber;
}

/*!----------------------------------------------------------------------------
*/
void StateGraph::printState( std::ostream& rOut, const int tabs )
{
   StateCollector::printTabs( rOut, tabs );
   printName( rOut );

   if( !m_vpAttributes.empty() )
      printAttr( rOut, m_vpAttributes );

   rOut << ";\n";
}

/*!----------------------------------------------------------------------------
*/
void StateGraph::printTransitions( std::ostream& rOut, const int tabs )
{
   if( m_pParent->noTransitions() )
      return;

#ifdef CONFIG_PRINT_CALLER_LIST
   for( auto& pCaller : m_vpCallerList )
   {
      StateCollector::printTabs( rOut, tabs );
      pCaller->getState()->printName( rOut );
      rOut << " -> ";
      printName( rOut );
      if( !pCaller->getTransition()->getAttrList().empty() )
         printAttr( rOut, pCaller->getTransition()->getAttrList(),
                    !m_pParent->noTransitionLabels() );
      rOut << ";\n";
   }
#else
   for( auto& pTransition : m_vpTransitions )
   {
      StateCollector::printTabs( rOut, tabs );
      printName( rOut );
      rOut << " -> ";
      pTransition->getTargetState()->printName( rOut );
      if( !pTransition->getAttrList().empty() )
         printAttr( rOut, pTransition->getAttrList(),
            !m_pParent->noTransitionLabels() );
      rOut << ";\n";
   }
#endif
}


/*!----------------------------------------------------------------------------
*/
void StateGraph::printAttr( std::ostream& rOut, const ATTR_LIST_T& rvpAttributes,
                            bool printLabel )
{
   bool next = false;
   rOut << " [";
   for( auto& pAttribute : rvpAttributes )
   {
      if( DotKeywords::getKategory( pAttribute->first ) != DotKeywords::DOT )
          continue;
      if( DotKeywords::getId( pAttribute->first ) == DotKeywords::LABEL &&
         !printLabel )
         continue;
      if( next )
         rOut << ", ";
      next = true;
      rOut << DotKeywords::getKeyWord( pAttribute->first ) << " = "
           << *pAttribute->second;
   }
   rOut << ']';
}

/*!----------------------------------------------------------------------------
*/
void StateGraph::addAttribute( const std::string& rKey, const std::string& rValue )
{
   m_vpAttributes.push_back( makeAttribute( DotKeywords::c_nodeAttributes, rKey, rValue ) );
}

/*!----------------------------------------------------------------------------
*/
StateGraph::~StateGraph( void )
{
   DEBUG_MESSAGE( "Destructor of \"" << m_name << "\"" );
   for( auto& pTransition : m_vpTransitions )
   {
      for( auto& pAttribute : pTransition->getAttrList() )
      {
         if( pAttribute->second != nullptr )
            delete pAttribute->second;
         delete pAttribute;
      }
      delete pTransition;
   }
#ifdef CONFIG_PRINT_CALLER_LIST
   for( auto& pCaller : m_vpCallerList )
      delete pCaller;
#endif
}

/*!----------------------------------------------------------------------------
*/
const ATTR_T* StateGraph::makeAttribute( const DotKeywords::DOT_ATTR_LIST_T& rAttrGroup,
                                   const std::string& rKey,
                                   const std::string& rValue )
{
   const DotKeywords::DOT_ATTR_ITEM_T* pAttr = DotKeywords::find( rAttrGroup, rKey );
   assert( pAttr != nullptr );
   return new ATTR_T( *pAttr, new std::string(rValue) );
}

/*!----------------------------------------------------------------------------
 * @note Recursive call
 */
void StateGraph::setFsm( int fsmNumber )
{
    assert( fsmNumber > 0 );

    if( m_fsmNumber == 0 )
    {
       DEBUG_MESSAGE( "Set fsmNumber-number " << fsmNumber << " for " << m_name );
       m_fsmNumber = fsmNumber;
       for( auto& pTransition : m_vpTransitions )
          pTransition->getTargetState()->setFsm( fsmNumber );
    }
}

/*!----------------------------------------------------------------------------
 * @note Recursive call
 */
bool StateGraph::_obtainFsmNumberIfEqual( int fsmNumber )
{
    if( fsmNumber == m_fsmNumber )
       return true;

    if( m_fsmNumber != 0 )
       return false;
 
    if( m_visited )
       return false;
    m_visited = true;

    bool ret = false;
    for( auto& pTransition : m_vpTransitions )
    {
       assert( dynamic_cast<StateGraph*>(pTransition->getTargetState()) != nullptr );
       DEBUG_MESSAGE( "Name " << m_name );
       if( pTransition->getTargetState() == this )
           continue;
       if( pTransition->getTargetState()->_obtainFsmNumberIfEqual( fsmNumber ) )
       {
          DEBUG_MESSAGE( "obtain fsmNumber-number " << fsmNumber << " for " << m_name );
          m_fsmNumber = fsmNumber;
          ret = true;
       }
    }
    if( m_fsmNumber == 0 )
        return ret;
    for( auto& pTransition : m_vpTransitions )
    {
       if( pTransition->getTargetState() == this )
          continue;
       if( pTransition->getTargetState()->m_fsmNumber == 0 )
       {
          DEBUG_MESSAGE( "set fsmNumber-number " << m_fsmNumber << " from " <<
                          m_name << " to " << pTransition->getTargetState()->m_name );
          pTransition->getTargetState()->m_fsmNumber = m_fsmNumber;
       }
    }
    return ret;
}

/*!----------------------------------------------------------------------------
*/
std::string* StateGraph::findGroupName( void )
{
   for( auto& pAttr : m_vpAttributes )
   {
      if( DotKeywords::getKategory( pAttr->first ) != DotKeywords::DOCFSM )
         continue;
      if( DotKeywords::getId( pAttr->first ) != DotKeywords::GROUP )
         continue;
      return pAttr->second;
   }
   return nullptr;
}

/*!----------------------------------------------------------------------------
*/
void StateGraph::generateTransitionTooltipFromLabel( void )
{
   for( auto& pTransition: m_vpTransitions )
      pTransition->generateTooltipFromLabel();
}

//================================== EOF ======================================
