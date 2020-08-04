#include <stdio.h>
#include <unistd.h>
#include <assert.h>

// gcc fsm_switch_case_entry_exit_docfsm.c -o fsm
// docfsm fsm_switch_case_entry_exit_docfsm.c | display

#define FSM_DECLARE_STATE_XE( name, attr... ) name

typedef enum
{
    _NO_STATE,
    FSM_DECLARE_STATE_XE( TICK, color=red ),
    FSM_DECLARE_STATE_XE( TACK, color=blue )
} STATE_T;

struct FSM_T
{
   STATE_T newState;
   STATE_T currentState;
   void*   pPrivateData;
};

#define FSM_TRANSITION( _newState, attr... ) pFsm->newState = _newState
#define FSM_TRANSITION_SELF( attr... )

void executeFsm( struct FSM_T* pFsm )
{
   int* pMyCounter = pFsm->pPrivateData;

   if( pFsm->newState != pFsm->currentState )
   {
      switch( pFsm->newState ) /* Entry actions */
      {

         case TICK:
         {
            printf( "entry TICK\n" );
            *pMyCounter = 5;
            break;
         }

         case TACK:
         {
            printf( "entry TACK\n" );
            *pMyCounter = 10;
            break;
         }
         default: assert( 0 ); break;
      }
      pFsm->currentState = pFsm->newState;
   }

   switch( pFsm->currentState ) /* Do actions */
   {
      case TICK:
      {
         printf( "do TICK counter: %d\n", *pMyCounter );
         if( *pMyCounter == 0 )
         {
            FSM_TRANSITION( TACK, label='counter == 0', color=blue );
            break;
         }
         (*pMyCounter)--;
         FSM_TRANSITION_SELF( TICK, label='counter != 0' );
         break;
      }

      case TACK:
      {
         printf( "do TACK counter: %d\n", *pMyCounter );
         if( *pMyCounter == 0 )
         {
            FSM_TRANSITION( TICK, label='counter == 0', color=red );
            break;
         }
         (*pMyCounter)--;
         FSM_TRANSITION_SELF( label='counter != 0' );
         break;
      }
      default: assert( 0 ); break;
   }

   if( pFsm->newState != pFsm->currentState )
   {
      switch( pFsm->currentState ) /* Exit actions */
      {
         case TICK:
         {
            printf( "exit TICK\n" );
            break;
         }
 
         case TACK:
         {
            printf( "exit TACK\n" );
            break;
         }
         default: assert( 0 ); break;
      }
   }
}

#define FSM_INIT_FSM( fsm, privateData, startState, attr... ) \
   initFsm( &fsm, &privateData, startState )

void initFsm( struct FSM_T* pFsm, void* pPrivate, STATE_T startState )
{
    pFsm->newState = startState;
    pFsm->currentState = _NO_STATE;
    pFsm->pPrivateData = pPrivate;
}

int main( void )
{
   int i;
   int myCounter = 10;
   struct FSM_T myFsm;

   printf( "Test of %s\n", __FILE__ ); 

   FSM_INIT_FSM( myFsm, myCounter, TICK, label='Start' );

   for( i = 0; i < 100; i++ )
   {
      executeFsm( &myFsm );
      sleep( 1 );
   }
   return 0;
}
