/*****************************************************************************/
/*                                                                           */
/*!      @brief Module stores and administrates the DocFsm-keywords          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    df_keyword_pool.cpp                                             */
/*! @see     df_keyword_pool.hpp                                             */
/*! @author  Ulrich Becker                                                   */
/*! @date    17.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#include "df_docfsm.hpp"
#include "df_keyword_pool.hpp"

using namespace DocFsm;

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::OverwriteTransition::OverwriteTransition( KeywordPool* pParent )
   :Option( pParent )
{
   m_shortOpt = 't';
   m_longOpt  = "transition";
   m_helpText = "Overwrites the list of state transition keywords"
                " by the in PARAM given keyword.\n"
                "NOTE: For each new keyword use a separate option-label.\n"
                "E.g.: -t FSM_MY_TRANSITION -t FSM_MY_TRANSITION_NEXT ...";
}

/*!----------------------------------------------------------------------------
*/
int KeywordPool::OverwriteTransition::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_vTransitionKeywords.push_back( poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::OverwriteDeclare::OverwriteDeclare( KeywordPool* pParent )
   :Option( pParent )
{
   m_shortOpt = 'd';
   m_longOpt  = "declare";
   m_helpText = "Overwrites the list of state deceleration keywords"
                " by the in PARAM given keyword.\n"
                "NOTE: For each new keyword use a separate option-label.\n"
                "E.g.: -d FSM_MY_DECLARE_STATE -d FSM_MY_DECLARE_STATE_X ...";
}

/*!----------------------------------------------------------------------------
*/
int KeywordPool::OverwriteDeclare::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_vDeclareKeywords.push_back( poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::OverwriteInitial::OverwriteInitial( KeywordPool* pParent )
   :Option( pParent )
{
   m_shortOpt = 'i';
   m_longOpt  = "init";
   m_helpText = "Overwrites the list of state initialization keywords"
                " by the in PARAM given keyword.\n"
                "NOTE: For each new keyword use a separate option-label.\n"
                "E.g.: -i FSM_MY_DECLARE_STATE -i FSM_MY_DECLARE_STATE_X ...";
}

/*!----------------------------------------------------------------------------
*/
int KeywordPool::OverwriteInitial::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_vInitialKeywords.push_back( poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::OverwriteCall::OverwriteCall( KeywordPool* pParent )
   :Option( pParent )
{
   m_shortOpt = 'c';
   m_longOpt  = "call";
   m_helpText = "Overwrites the list of keywords for sub-FSM calling"
                " by the in PARAM given keyword.\n"
                "NOTE: For each new keyword use a separate option-label.\n"
                "E.g.: -c FSM_MY_CALL -c FSM_MY_CALL_NEXT ...";
}

/*!----------------------------------------------------------------------------
*/
int KeywordPool::OverwriteCall::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_vCallKeywords.push_back( poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::OverwriteReturn::OverwriteReturn( KeywordPool* pParent )
   :Option( pParent )
{
   m_shortOpt = 'r';
   m_longOpt  = "return";
   m_helpText = "Overwrites the list of keywords for return statements from"
                " sub-FSMs by the in PARAM given keyword.\n"
                "NOTE: For each new keyword use a separate option-label.\n"
                "E.g.: -r FSM_MY_RETURN -r FSM_MY_RETURN_NEXT ...";
}

/*!----------------------------------------------------------------------------
*/
int KeywordPool::OverwriteReturn::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->m_vReturnKeywords.push_back( poParser->getOptArg() );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::ListKeywords::ListKeywords( KeywordPool* pParent )
   :Option( pParent )
{
   m_hasArg   = NO_ARG;
   m_shortOpt = 'l';
   m_longOpt  = "list";
   m_helpText = "Shows all keywords and exit.";
}

/*!----------------------------------------------------------------------------
*/
int KeywordPool::ListKeywords::onGiven( CLOP::PARSER* poParser )
{
   m_pParent->setDefaultsIfEmpty();

   m_pParent->listTransitionKeywords( std::cout );
   std::cout << "\n";
   m_pParent->listDeclareKeywords( std::cout );
   std::cout << "\n";
   m_pParent->listInitialKeywords( std::cout );
   std::cout << "\n";
   m_pParent->listCallKeywords( std::cout );
   std::cout << "\n";
   m_pParent->listReturnKeywords( std::cout );
   std::cout << std::endl;

   ::exit( EXIT_SUCCESS );
   return 0;
}

///////////////////////////////////////////////////////////////////////////////
/*!----------------------------------------------------------------------------
*/
KeywordPool::KeywordPool( CommandlineParser& rCommandlineParser )
   :m_overwriteTransition( this )
   ,m_overwriteDeclare( this )
   ,m_overwriteInitial( this )
   ,m_overwriteCall( this )
   ,m_overwriteReturn( this )
   ,m_oListKeywords( this )
{
   rCommandlineParser( m_overwriteTransition )
                     ( m_overwriteDeclare )
                     ( m_overwriteInitial )
                     ( m_overwriteCall )
                     ( m_overwriteReturn )
                     ( m_oListKeywords );
}

/*!----------------------------------------------------------------------------
*/
bool KeywordPool::setDefaultsIfEmpty( void )
{
   if( m_vTransitionKeywords.empty() )
   {
      m_vTransitionKeywords.push_back( "FSM_TRANSITION" );
      m_vTransitionKeywords.push_back( "FSM_TRANSITION_NEXT" );
   }

   if( m_vDeclareKeywords.empty() )
   {
      m_vDeclareKeywords.push_back( "FSM_DECLARE_STATE" );
      m_vDeclareKeywords.push_back( "FSM_DECLARE_STATE_X" );
      m_vDeclareKeywords.push_back( "FSM_DECLARE_STATE_E" );
      m_vDeclareKeywords.push_back( "FSM_DECLARE_STATE_XE" );
   }

   if( m_vInitialKeywords.empty() )
   {
      m_vInitialKeywords.push_back( "FSM_INIT_FSM" );
   }

   if( m_vCallKeywords.empty() )
   {
      m_vCallKeywords.push_back( "FSM_CALL" );
      m_vCallKeywords.push_back( "FSM_CALL_NEXT" );
   }

   if( m_vReturnKeywords.empty() )
   {
      m_vReturnKeywords.push_back( "FSM_RETURN" );
      m_vReturnKeywords.push_back( "FSM_RETURN_NEXT" );
   }

   return multipleCheck();
}

/*!----------------------------------------------------------------------------
*/
bool KeywordPool::multipleCheck( void )
{
   const std::vector<const CONTAINER_T*> allWords =
   {
      &m_vTransitionKeywords,
      &m_vDeclareKeywords,
      &m_vInitialKeywords,
      &m_vCallKeywords,
      &m_vReturnKeywords
   };

   bool ret = false;
   for( auto i = allWords.begin(); i < allWords.end()-1; i++ )
   {
      for( auto j = i+1; j < allWords.end(); j++ )
      {
         for( auto w = (*j)->begin(); w < (*j)->end(); w++ )
         {
            if( isInList( **i, *w ) )
            {
               ERROR_MESSAGE( "Multiple keyword \"" << *w << "\" found!" );
               ret = true;
            }
         }
      }
   }

   return ret;
}


/*!----------------------------------------------------------------------------
*/
void KeywordPool::listKeywords( std::ostream& rOut, const CONTAINER_T& rvKeywords )
{
   bool notFirst = false;
   for( auto& i : rvKeywords )
   {
      if( notFirst )
         rOut << ", ";
      notFirst = true;
      rOut << i;
   }
}

/*!----------------------------------------------------------------------------
*/
bool KeywordPool::isInList( const CONTAINER_T& rvKeywords, const std::string& rWord )
{
   for( auto& i : rvKeywords )
   {
      if( i == rWord )
         return true;
   }
   return false;
}

/*!----------------------------------------------------------------------------
*/
KeywordPool::TYPE_T KeywordPool::determineTransitionType( const std::string& rWord )
{
   if( isOneOfTransitionKeyWords( rWord ) )
      return TRANSITION;

   if( isOneOfReturnKeyWords( rWord ) )
      return RETURN;

   if( isOneOfCallKeyWords( rWord ) )
      return CALL;

   return NON;
}

//================================== EOF ======================================
