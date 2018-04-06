/*****************************************************************************/
/*                                                                           */
/*!             @brief Handling and verifying keywords of DOT                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_dot_keywords.hpp                                             */
/*! @see     df_dot_keywords.cpp                                             */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_DOT_KEYWORDS_HPP
#define _DF_DOT_KEYWORDS_HPP

#ifndef __DOCFSM__
 #include <tuple>
 #include <sstream>
 #include <assert.h>
#endif
#include "df_commandline.hpp"

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
class DotKeywords
{
   class DotKeywordsOption: public CLOP::OPTION_V
   {
   protected:
      DotKeywords*      m_pParent;
      DotKeywordsOption( DotKeywords* pParent )
         :m_pParent( pParent )
      {
         m_hasArg   = NO_ARG;
         m_shortOpt = '\0';
      }
   };

   class OptionListNodeWords: public DotKeywordsOption
   {
   public:
      OptionListNodeWords( DotKeywords* pParent );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class OptionListEdgeWords: public DotKeywordsOption
   {
   public:
      OptionListEdgeWords( DotKeywords* pParent );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class OptionListGraphWords: public DotKeywordsOption
   {
   public:
      OptionListGraphWords( DotKeywords* pParent );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   OptionListNodeWords   m_oOptionListNodeWords;
   OptionListEdgeWords   m_oOptionListEdgeWords;
   OptionListGraphWords  m_oOptionListGraphWords;

public:
   enum ID_T
   {
      NOTHING,
      LABEL,
      COLOR,
      FONTCOLOR,
      FILLCOLOR,
      GROUP,
      SHAPE,
      STYLE,
      URL
   };

   enum KATEGORY_T
   {
      DOT,   //!< Attributes for DOT
      DOCFSM //!< Attributes for DocFsm
   };

   typedef std::tuple< std::string, std::string, ID_T, KATEGORY_T > DOT_ATTR_ITEM_T;
   typedef std::vector<DOT_ATTR_ITEM_T>                             DOT_ATTR_LIST_T;

   const static DOT_ATTR_LIST_T c_edgeAttributes;
   const static DOT_ATTR_LIST_T c_nodeAttributes;
   const static DOT_ATTR_LIST_T c_graphAttributes;

   DotKeywords( CommandlineParser& );

   static const std::string& getKeyWord( const DOT_ATTR_ITEM_T& rItem )
   {
      return std::get<0>( rItem );
   }

   static const std::string& getHelpText( const DOT_ATTR_ITEM_T& rItem )
   {
      return std::get<1>( rItem );
   }

   static const ID_T getId( const DOT_ATTR_ITEM_T& rItem )
   {
      return std::get<2>( rItem );
   }

   static const KATEGORY_T getKategory( const DOT_ATTR_ITEM_T& rItem )
   {
      return std::get<3>( rItem );
   }

   static const DOT_ATTR_ITEM_T* find( const DOT_ATTR_LIST_T&, const std::string& );

   static const DOT_ATTR_ITEM_T* findNodeWord( const std::string& w )
   {
      return find( c_nodeAttributes, w );
   }

   static const DOT_ATTR_ITEM_T* findEdgeWord( const std::string& w )
   {
      return find( c_edgeAttributes, w );
   }

   static const DOT_ATTR_ITEM_T* findGraphWord( const std::string& w )
   {
      return find( c_graphAttributes, w );
   }

   void list( const DOT_ATTR_LIST_T& rList, bool verbose );

};

} // End namespace DocFsm
#endif // ifndef _DF_DOT_KEYWORDS_HPP
//================================== EOF ======================================
