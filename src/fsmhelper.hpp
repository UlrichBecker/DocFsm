/*****************************************************************************/
/*                                                                           */
/*! @brief Module provides some wrapper macros for interpreting FSM's by     */
/*                              DocFsm                                       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*! @file    fsmhelper.hpp                                                   */
/*! @author  Ulrich Becker                                                   */
/*! @date    11.11.2017                                                      */
/*  Updates:                                                                 */
/*****************************************************************************/
#ifndef _FSMHELPER_HPP
#define _FSMHELPER_HPP

/*
 *
 */
#ifdef _FSM_DEBUG_INFO
   #include <typeinfo>
//   #define DEBUG_MESSAGE( args... ) std::cerr << "DBG: " << args << std::endl
   #define DEBUG_FSM_MESSAGE( args... ) std::cerr << "DBG FSM: " << args << std::endl
#else
//   #define DEBUG_MESSAGE( args... )
   #define DEBUG_FSM_MESSAGE( args... )
#endif

/*
 * Helper-macros for the finite state machine to make the FSM better readable
 * and easier to find the states e.g. via grep.
 */
#define _FSM_DECLARE_STATE( parent_t, name )          \
    class name : public State                         \
    {                                                 \
    public:                                           \
        name( parent_t* pParent )                     \
            : State( pParent ) {}                     \
        bool onEntry( void ) override;                \
        bool onDo( char ) override;                   \
    }  m_sto ## name

#define FSM_IMPORT_PARENT_VOID_FUNCTION( name )       \
    void name( void )                                 \
    {                                                 \
        m_pParent->name();                            \
    }

#define FSM_IMPORT_PARENT_RET_FUNCTION( ret_t, name ) \
    ret_t name( void )                                \
    {                                                 \
        return m_pParent->name();                     \
    }

#define FSM_INIT_STATE( name, l... ) m_sto ## name( this )

#define FSM_INIT_FSM( startState, l... ) m_pCurrentState( &m_sto ## startState )

#define FSM_TRANSITION( newState, l... )                    \
    return __transition( m_pParent->m_sto ## newState )

#define FSM_EXTERN_TRANSITION( newState, l... )  transition( m_sto ## newState )

///////////////////////////////////////////////////////////////////////////////
template <class PARENT_T, typename ARG_T, typename RET_T = bool>
class FSM_STATE
{
public:
   PARENT_T* m_pParent;
   FSM_STATE( PARENT_T* pParent ): m_pParent( pParent ) {}
   virtual ~FSM_STATE( void ) {}
   virtual RET_T onEntry( void ) = 0;
   virtual RET_T onDo( ARG_T ) = 0;
   virtual RET_T onExit( void ) { return static_cast<RET_T>(0); }
};

#endif //ifndef _FSMHELPER_HPP
//================================== EOF ======================================
