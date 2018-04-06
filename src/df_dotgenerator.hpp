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
public:
   TransitionGraph( StateGraph* pTargetState )
   :   m_pTargetState( pTargetState )
   {}
   StateGraph*             m_pTargetState;
   ATTR_LIST_T             m_vpAttributes;
};

///////////////////////////////////////////////////////////////////////////////
class StateCollector;

class StateGraph
{
public:
   StateCollector*               m_pParent;
   int                           m_fsmNumber;
   int                           m_clusterNumber;
   bool                          m_visited;
   std::string                   m_name;
   std::vector<TransitionGraph*> m_vpTransitions;
   ATTR_LIST_T                   m_vpAttributes;

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

private:
   bool _obtainFsmNumberIfEqual( int fsmNumber );
   void printName( std::ostream& rOut );
};

} // End namespace DocFsm
#endif //ifndef _DF_DOTGENERATOR_HPP
//================================== EOF ======================================
