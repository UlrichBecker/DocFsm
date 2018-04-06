/*****************************************************************************/
/*                                                                           */
/*!  @brief Helper-macros to implement a finite state machine (FSM) in       */
/*!         C++ classes.                                                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    fsm_class_implementer.hpp                                       */
/*! @author  Ulrich Becker                                                   */
/*! @date    25.12.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _FSM_CLASS_IMPLEMENTER_HPP
#define _FSM_CLASS_IMPLEMENTER_HPP

#ifndef __cplusplus
   #error "This header is for C++ projects only!" 
#endif

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_DECLARE_STATE
  #undef FSM_DECLARE_STATE
#endif
#define FSM_DECLARE_STATE( name, attr... )    \
   STATE_T m_ ## name;                        \
   bool name ( EVENT_T );

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_DECLARE_STATE_X
  #undef FSM_DECLARE_STATE_X
#endif
#define FSM_DECLARE_STATE_X( name, attr... )  \
   STATE_T m_ ## name;                        \
   bool name ( EVENT_T );                     \
   void name ## Exit ( void );

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_DECLARE_STATE_E
  #undef FSM_DECLARE_STATE_E
#endif
#define FSM_DECLARE_STATE_E( name, attr... )  \
   STATE_T m_ ## name;                        \
   bool name ( EVENT_T );                     \
   void name ## Entry ( void );

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_DECLARE_STATE_XE
   #undef FSM_DECLARE_STATE_XE
#endif
#define FSM_DECLARE_STATE_XE( name, attr... ) \
   STATE_T m_ ## name;                        \
   bool name ( EVENT_T );                     \
   void name ## Exit ( void );                \
   void name ## Entry ( void );

/*!----------------------------------------------------------------------------
 */
#define _CALL_MEMBER_FUNC( pO, pF)  ((pO)->*(pF))
#define CALL_MEMBER_FUNC( pF ) _CALL_MEMBER_FUNC( this, pF )

#ifdef _DEBUG_FSM_IMPLEMENTER
  #define __FSM_STATE_NAME__ const char* m_name;
  #define __FSM_DEBUG_ARG__  const char* name,
  #define __FSM_DEBUG_INIT__ m_name( name ),
#else
  #define __FSM_STATE_NAME__
  #define __FSM_DEBUG_ARG__
  #define __FSM_DEBUG_INIT__
#endif

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_CLASS_IMPLEMENT
  #undef FSM_CLASS_IMPLEMENT
#endif
#define FSM_CLASS_IMPLEMENT( className )                        \
   typedef  void (className::*OnEntry)(void);                   \
   typedef  bool (className::*OnDo)(EVENT_T);                   \
   typedef  void (className::*OnExit)(void);                    \
   struct STATE_T                                               \
   {                                                            \
      OnEntry m_onEntry;                                        \
      OnDo    m_onDo;                                           \
      OnExit  m_onExit;                                         \
      __FSM_STATE_NAME__                                        \
      STATE_T( __FSM_DEBUG_ARG__ OnDo onDo,                     \
               OnEntry onEntry = nullptr,                       \
               OnExit onExit = nullptr )                        \
         :__FSM_DEBUG_INIT__ m_onEntry( onEntry )               \
         ,m_onDo( onDo )                                        \
         ,m_onExit( onExit )                                    \
         {}                                                     \
   };                                                           \
   STATE_T* m_pCurrentState;                                    \
   bool fsmDo( EVENT_T ev )                                     \
   {                                                            \
      return CALL_MEMBER_FUNC( m_pCurrentState->m_onDo )( ev ); \
   }                                                            \
   void fsmTransition( STATE_T& rNewState );

#ifdef _DEBUG_FSM_IMPLEMENTER
  #define FSM_INIT_OBJ( class, name ) m_ ## name( #name, &class::name )
  #define FSM_INIT_OBJ_X( class, name ) m_ ## name( #name, &class::name,  \
     nullptr, &class::name ## Exit )
  #define FSM_INIT_OBJ_E( class, name ) m_ ## name( #name, &class::name,  \
     &class::name ## Entry )
  #define FSM_INIT_OBJ_XE( class, name ) m_ ## name( #name, &class::name, \
     &class::name ## Entry, &class::name ## Exit )
#else
  #define FSM_INIT_OBJ( class, name ) m_ ## name( &class::name )
  #define FSM_INIT_OBJ_X( class, name ) m_ ## name( &class::name, nullptr, \
     &class::name ## Exit )
  #define FSM_INIT_OBJ_E( class, name ) m_ ## name( &class::name, \
     &class::name ## Entry )
  #define FSM_INIT_OBJ_XE( class, name ) m_ ## name(&class::name, \
     &class::name ## Entry, &class::name ## Exit )
#endif

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_RETURN
  #undef FSM_RETURN
#endif
#define FSM_RETURN( l... ) fsmReturn()

/*!----------------------------------------------------------------------------
 */
#define FSM_IS( state ) (m_pCurrentState == &(m_ ## state))

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_INIT_FSM
  #undef FSM_INIT_FSM
#endif
#define FSM_INIT_FSM( state, attr... ) m_pCurrentState = &(m_ ## state)

/*!----------------------------------------------------------------------------
 */
#ifdef FSM_TRANSITION
   #undef FSM_TRANSITION
#endif
#define FSM_TRANSITION( _newState, attr... ) fsmTransition( m_ ## _newState )

#ifndef FSM_INIT_OBJ
   #undef FSM_INIT_OBJ
#endif

#endif // ifndef _FSM_CLASS_IMPLEMENTER_HPP
//================================== EOF ======================================
