#include <stdio.h>
#include <unistd.h>
#include <assert.h>

/*
 * gcc fsm_switch_case_entry_exit_origin.c -o fsm
 */

typedef enum
{
    _NO_STATE,
    TICK,
    TACK
} STATE_T;

struct FSM_T
{
   STATE_T newState;
   STATE_T currentState;
   void*   pPrivateData;
};

void executeFsm( struct FSM_T* pFsm )
{
   int* pMyCounter = pFsm->pPrivateData;

   if( pFsm->newState != pFsm->currentState )
   {
      switch( pFsm->newState ) /* Entry actions */
      {
         case TICK:
            printf( "entry TICK\n" );
            *pMyCounter = 5;
            break;
         case TACK:
            printf( "entry TACK\n" );
            *pMyCounter = 10;
            break;
         default: assert( 0 ); break;
      }
      pFsm->currentState = pFsm->newState;
   }

   switch( pFsm->currentState ) /* Do actions */
   {
      case TICK:
         printf( "do TICK counter: %d\n", *pMyCounter );
         if( *pMyCounter == 0 )
         {
            pFsm->newState = TACK;
            break;
         }
         (*pMyCounter)--;
         break;
      case TACK:
         printf( "do TACK counter: %d\n", *pMyCounter );
         if( *pMyCounter == 0 )
         {
            pFsm->newState = TICK;
            break;
         }
         (*pMyCounter)--;
         break;
      default: assert( 0 ); break;
   }

   if( pFsm->newState != pFsm->currentState )
   {
      switch( pFsm->currentState ) /* Exit actions */
      {
         case TICK:
            printf( "exit TICK\n" );
            break;
         case TACK:
            printf( "exit TACK\n" );
            break;
         default: assert( 0 ); break;
      }
   }
}

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

   initFsm( &myFsm, &myCounter, TICK );

   for( i = 0; i < 100; i++ )
   {
      executeFsm( &myFsm );
      sleep( 1 );
   }
   return 0;
}