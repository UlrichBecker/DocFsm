/*****************************************************************************/
/*                                                                           */
/*!      @brief Module recognized and collects the state-keywords (macros)   */
/*!             in the C/C++ source-code.                                    */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_state_collector.hpp                                          */
/*! @see     df_state_collector.cpp                                          */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_STATE_COLLECTOR_HPP
#define _DF_STATE_COLLECTOR_HPP

#ifndef __DOCFSM__
 #include <list>
 #include "df_keyword.hpp"
 #include "df_keyword_pool.hpp"
 #include "df_dotgenerator.hpp"
 #include "df_attribute_reader.hpp"
 #include "df_dot_keywords.hpp"
#endif

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
class StateCollector: public KeywordInterpreter
{
   static const std::string c_strLabel;
   typedef  std::list <StateGraph*> STATES;

   struct GROUP
   {
      STATES              m_vpStates;
      std::string*  m_pName;

      GROUP( std::string* pName = nullptr );
      ~GROUP( void );
      void print( std::ostream& rOut, int tabs );
      bool haveName( void );
      const std::string& getName( void ) const
      {
         assert( m_pName != nullptr );
         return *m_pName;
      }
      bool doesNotBelongToMe( std::string* pName );
      bool doesContaining( StateGraph* );
      void addStatePtrIfNotAlreadyContaining( StateGraph* );
   };

   typedef std::list <GROUP*> GROUPS;

   struct FSM
   {
      GROUPS   m_vpGroups;
      FSM( void );
      ~FSM( void );
      STATES& getStateList( void )
      {
         assert( !m_vpGroups.empty() );
         return m_vpGroups.back()->m_vpStates;
      }
      int splitInGroups( void );
      void print( std::ostream& rOut, const std::string& clusterName, int tabs );
   };

   typedef std::vector<FSM*> FSM_LIST;

   struct MODULE
   {
      const std::string&  m_name;
      FSM_LIST            m_vpFsm;
      ATTR_LIST_T         m_vpAttributes;
      MODULE( const std::string& name );
      ~MODULE( void );
      int splitInClusters( void );
      int splitInGroups( void );
      int isATarget( const StateGraph* pStateGraph );
   };

   typedef std::vector <MODULE*> MODULE_V;

   enum TO_COLLECT_T
   {
      COLLECT_TRANSITIONS,
      COLLECT_INITIALIZATIONS,
      COLLECT_DECLARATIONS
   };

   class Option: public CLOP::OPTION_V
   {
   protected:
      StateCollector*  m_pParent;
      Option( StateCollector*  pParent )
         :m_pParent( pParent ) {}
   };

   class SetGraphAttributes: public Option
   {
   public:
      SetGraphAttributes( StateCollector* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class SetNodeAttributes: public Option
   {
   public:
      SetNodeAttributes( StateCollector* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class SetEdgeAttributes: public Option
   {
   public:
      SetEdgeAttributes( StateCollector* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class SetSingleGraph: public Option
   {
   public:
      SetSingleGraph( StateCollector* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class SetNoStateGroups: public Option
   {
   public:
      SetNoStateGroups( StateCollector* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class SetNoFsmGroups: public Option
   {
   public:
      SetNoFsmGroups( StateCollector* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   SetGraphAttributes                  m_setGraphAttributes;
   SetNodeAttributes                   m_setNodeAttributes;
   SetEdgeAttributes                   m_setEdgeAttributes;
   SetSingleGraph                      m_setSingleGraph;
   SetNoStateGroups                    m_setNoStateGroups;
   SetNoFsmGroups                      m_setNoFsmGroups;
   ATTR_LIST_T                         m_vpGraphAttributes;
   ATTR_LIST_T                         m_vpNodeAttributes;
   ATTR_LIST_T                         m_vpEdgeAttributes;
   TO_COLLECT_T                        m_toCollect;
   KeywordPool&                        m_rKeywords;
   bool                                m_parseArgument;
   bool                                m_isSingle;
   bool                                m_noStateGroups;
   bool                                m_noFsmGroups;
   int                                 m_entryCount;
   MODULE_V                            m_vpModules;
   AttributeReader                     m_oAttributeReader;
   const DotKeywords::DOT_ATTR_ITEM_T* m_pLabelAttribute;

public:
   StateCollector( SourceBrowser&, KeywordPool&, CommandlineParser& );
   virtual ~StateCollector( void );

   bool collect( const std::string& rName );
   bool operator()( const std::string& rName )
   {
      return collect( rName );
   }

#if 0 //RFU
   int isATarget( const StateGraph* pStateGraph )
   {
      assert( !m_vpModules.empty() );
      return m_vpModules.back()->isATarget( pStateGraph );
   }
#endif

   bool isEmpty( void ) const
   {
      return m_vpModules.empty();
   }

   STATES& get( void ) const
   {
      assert( !m_vpModules.empty() );
      assert( !m_vpModules.back()->m_vpFsm.empty() );
      return m_vpModules.back()->m_vpFsm.back()->getStateList();
   }

   StateGraph* find( const std::string& );

   void splitInClusters( void );
   void splitInGroups( void );

   void print( std::ostream& rOut );

   KeywordPool& getKeywords( void )
   {
      return m_rKeywords;
   }

   AttributeReader& getAttributeReader( void )
   {
      return m_oAttributeReader;
   }

   bool browse( TO_COLLECT_T toCollect )
   {
      m_toCollect = toCollect;
      return KeywordInterpreter::browse();
   }

   const DotKeywords::DOT_ATTR_ITEM_T* getLabelAttribute( void )
   {
      return m_pLabelAttribute;
   }

   StateGraph* addIfNotAlreadyDone( const std::string& rWord );

   static std::string baseFileName( const std::string& rStr );
   static std::string stripFileName( const std::string& rStr );

   static void printTabs( std::ostream& rOut, int tabs );

protected:
   void onRecognized( std::string word ) override;

private:
   StateGraph* add( const std::string& rWord );

   bool addGlobalAttribute( const DotKeywords::DOT_ATTR_LIST_T& rDotAttrList,
                            ATTR_LIST_T& rAttr,
                            const std::string& rStr );

   bool addGraphAttribute( const std::string& rStr )
   {
      return addGlobalAttribute( DotKeywords::c_graphAttributes,
                                 m_vpGraphAttributes, rStr );
   }

   bool addNodeAttribute( const std::string& rStr )
   {
      return addGlobalAttribute( DotKeywords::c_nodeAttributes,
                                 m_vpNodeAttributes, rStr );
   }

   bool addEdgeAttribute( const std::string& rStr )
   {
      return addGlobalAttribute( DotKeywords::c_edgeAttributes,
                                 m_vpEdgeAttributes, rStr );
   }

   void setGlobalAttributesIfEmpty( void );
   bool prepareEntryPoint( StateGraph* poTargetStade );
   bool readStateAttributes( StateGraph* poTargetStade );
};

} // End namespace DocFsm
#endif // ifndef _DF_STATE_COLLECTOR_HPP
//================================== EOF ======================================
