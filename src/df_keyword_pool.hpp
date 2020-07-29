/*****************************************************************************/
/*                                                                           */
/*!      @brief Module stores and administrates the DocFsm-keywords          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_keyword_pool.hpp                                             */
/*! @see     df_keyword_pool.cpp                                             */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _DF_KEYWORD_POOL_HPP
#define _DF_KEYWORD_POOL_HPP

#include "df_commandline.hpp"

namespace DocFsm
{

///////////////////////////////////////////////////////////////////////////////
class KeywordPool
{
public:
   using CONTAINER_T = std::vector<std::string>;
   enum TYPE_T
   {
      NON,
      TRANSITION,
   #ifdef CONFIG_USE_KEYWORD_TRANSITION_SELF
      TRANSITION_SELF,
   #endif
      CALL,
      RETURN
   };

private:
   class Option: public CLOP::OPTION_V
   {
   protected:
      KeywordPool*  m_pParent;
      Option( KeywordPool* pParent )
         :m_pParent( pParent )
      {
         m_hasArg = REQUIRED_ARG;
      }
   };

   class OverwriteTransition: public Option
   {
   public:
      OverwriteTransition( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

#ifdef CONFIG_USE_KEYWORD_TRANSITION_SELF
   class OverwriteTransitionSelf: public Option
   {
   public:
      OverwriteTransitionSelf( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };
#endif

   class OverwriteDeclare: public Option
   {
   public:
      OverwriteDeclare( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class OverwriteInitial: public Option
   {
   public:
      OverwriteInitial( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class OverwriteCall: public Option
   {
   public:
      OverwriteCall( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class OverwriteReturn: public Option
   {
   public:
      OverwriteReturn( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   class ListKeywords: public Option
   {
   public:
      ListKeywords( KeywordPool* );
      int onGiven( CLOP::PARSER* poParser ) override;
   };

   CONTAINER_T         m_vTransitionKeywords;
#ifdef CONFIG_USE_KEYWORD_TRANSITION_SELF
   CONTAINER_T         m_vTransitionSelfKeywords;
#endif
   CONTAINER_T         m_vDeclareKeywords;
   CONTAINER_T         m_vInitialKeywords;
   CONTAINER_T         m_vCallKeywords;
   CONTAINER_T         m_vReturnKeywords;

   OverwriteTransition m_overwriteTransition;
#ifdef CONFIG_USE_KEYWORD_TRANSITION_SELF
   OverwriteTransitionSelf m_overwriteTransitionSelf;
#endif
   OverwriteDeclare    m_overwriteDeclare;
   OverwriteInitial    m_overwriteInitial;
   OverwriteCall       m_overwriteCall;
   OverwriteReturn     m_overwriteReturn;
   ListKeywords        m_oListKeywords;

public:
   KeywordPool( CommandlineParser& );

   bool setDefaultsIfEmpty( void );

   void listKeywords( std::ostream&, const CONTAINER_T& );

   void listTransitionKeywords( std::ostream& rOut )
   {
      listKeywords( rOut, m_vTransitionKeywords );
   #ifdef CONFIG_USE_KEYWORD_TRANSITION_SELF
      rOut << ", ";
      listKeywords( rOut, m_vTransitionSelfKeywords );
   #endif
   }

   void listDeclareKeywords( std::ostream& rOut )
   {
      listKeywords( rOut, m_vDeclareKeywords );
   }

   void listInitialKeywords( std::ostream& rOut )
   {
      listKeywords( rOut, m_vInitialKeywords );
   }

   void listCallKeywords( std::ostream& rOut )
   {
      listKeywords( rOut, m_vCallKeywords );
   }

   void listReturnKeywords( std::ostream& rOut )
   {
      listKeywords( rOut, m_vReturnKeywords );
   }

   bool isInList( const CONTAINER_T& rvKeywords, const std::string& rWord );

   bool isOneOfTransitionKeyWords( const std::string& word )
   {
      return isInList( m_vTransitionKeywords, word );
   }
   
#ifdef CONFIG_USE_KEYWORD_TRANSITION_SELF
   bool isOneOfTransitionSelfKeyWords( const std::string& word )
   {
      return isInList( m_vTransitionSelfKeywords, word );
   }
#endif

   bool isOneOfDeclareKeyWords( const std::string& word )
   {
      return isInList( m_vDeclareKeywords, word );
   }

   bool isOneOfInitialKeyWords( const std::string& word )
   {
      return isInList( m_vInitialKeywords, word );
   }

   bool isOneOfCallKeyWords( const std::string& word )
   {
      return isInList( m_vCallKeywords, word );
   }

   bool isOneOfReturnKeyWords( const std::string& word )
   {
      return isInList( m_vReturnKeywords, word );
   }

   TYPE_T determineTransitionType( const std::string& word );

private:
   bool multipleCheck( void );
};

} // End namespace DocFsm

#endif // ifndef _DF_KEYWORD_POOL_HPP
//================================== EOF ======================================

