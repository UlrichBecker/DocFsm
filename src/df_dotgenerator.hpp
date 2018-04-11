/*****************************************************************************/
/*                                                                           */
/*!      @brief Module creates the DOT-output ready for GRAPHVIZ             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_dotgenerator.hpp                                             */
/*! @see     df_dotgenerator.cpp                                             */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_DOTGENERATOR_HPP
#define _DF_DOTGENERATOR_HPP

#ifndef __DOCFSM__
 #include <assert.h>
 #include "df_keyword.hpp"
 #include "df_dot_keywords.hpp"
#endif

namespace DocFsm
{

typedef std::pair< const DotKeywords::DOT_ATTR_ITEM_T&, std::string* > ATTR_T;
typedef std::vector< const ATTR_T* > ATTR_LIST_T;

///////////////////////////////////////////////////////////////////////////////
class StateGraph;

class TransitionGraph
{
   StateGraph*             m_pTargetState;
   ATTR_LIST_T             m_vpAttributes;

public:
   TransitionGraph( StateGraph* pTargetState )
   :   m_pTargetState( pTargetState )
   {}
   ATTR_LIST_T& getAttrList( void ) { return m_vpAttributes; }
   StateGraph*  getTargetState( void ) { return m_pTargetState; }
};

///////////////////////////////////////////////////////////////////////////////
class StateCollector;

class StateGraph
{
public:
#ifdef CONFIG_PRINT_CALLER_LIST
   class Caller
   {
      StateGraph*      m_pState;
      TransitionGraph* m_pTransition;

   public:
      Caller( StateGraph* pSt, TransitionGraph* pTr )
         : m_pState( pSt )
         , m_pTransition( pTr )
         {}
      StateGraph*      getState( void ) { return m_pState; }
      TransitionGraph* getTransition( void ) { return m_pTransition; }
   };

   typedef std::vector<Caller*> CALLER_LIST_T;
#endif

   typedef std::vector<TransitionGraph*> TRANSITION_LIST_T;
   
private:
   StateCollector*               m_pParent;
   int                           m_fsmNumber;
   int                           m_clusterNumber;
   bool                          m_visited;
   std::string                   m_name;
   TRANSITION_LIST_T             m_vpTransitions;
   ATTR_LIST_T                   m_vpAttributes;
#ifdef CONFIG_PRINT_CALLER_LIST
   CALLER_LIST_T                 m_vpCallerList;
#endif

public:
   StateGraph( StateCollector* pParent, const std::string& name )
      :m_pParent( pParent )
      ,m_fsmNumber( 0 )
      ,m_clusterNumber( 0 )
      ,m_visited( false )
      ,m_name( name )
   {}
   ~StateGraph( void );


   void print( std::ostream&, const int=0 );
   static void printAttr( std::ostream& rOut, const ATTR_LIST_T& );
   void addAttribute( const std::string& rKey, const std::string& rValue );
   void addClusterNumber( int n );

   static const ATTR_T* makeAttribute( const DotKeywords::DOT_ATTR_LIST_T&,
                                 const std::string& rKey,
                                 const std::string& rValue );

   void setFsm( int fsmNumber );
   bool obtainFsmNumberIfEqual( int fsmNumber )
   {
      m_visited = false;
      return _obtainFsmNumberIfEqual( fsmNumber );
   }

   std::string* findGroupName( void );

#ifdef CONFIG_PRINT_CALLER_LIST
   void addCaller( StateGraph* pSt, TransitionGraph* pTr )
   {
      m_vpCallerList.push_back( new Caller( pSt, pTr ) );
   }

   CALLER_LIST_T& getCallerList( void )
   {
      return m_vpCallerList;
   }
#endif

   TRANSITION_LIST_T& getTransitionList( void )
   {
      return m_vpTransitions;
   }

   void addTransition( TransitionGraph* pTransition )
   {
      assert( pTransition != nullptr );
      m_vpTransitions.push_back( pTransition );
   #ifdef CONFIG_PRINT_CALLER_LIST
      assert( pTransition->getTargetState() != nullptr );
      pTransition->getTargetState()->addCaller( this, pTransition );
   #endif
   }

   std::string& getName( void ) { return  m_name; }
   int getFsmNumber( void ) const { return m_fsmNumber; }
   ATTR_LIST_T& getAttrList( void ) { return m_vpAttributes; }
private:
   bool _obtainFsmNumberIfEqual( int fsmNumber );
   void printName( std::ostream& rOut );
};

} // End namespace DocFsm
#endif //ifndef _DF_DOTGENERATOR_HPP
//================================== EOF ======================================
