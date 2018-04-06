#include <stdio.h>
#include <unistd.h>

// gcc fsm_func_ptr_entry_exit_docfsm.c -o fsm
// docfsm fsm_func_ptr_entry_exit_docfsm.c | display

struct FSM_T;

struct STATE_T
{
   void (*_onEntry)( void* pPrivateData );
   void (*_onDo)( struct FSM_T* pFsm );
   void (*_onExit)( void* pPrivateData );
};

struct FSM_T
{
   struct STATE_T* pCurrentState;
   struct STATE_T* pNewState;
   void* pPrivateData;
};

void fsmExecute( struct FSM_T* pFsm )
{
   if( pFsm->pCurrentState != pFsm->pNewState )
   {
      pFsm->pNewState->_onEntry( pFsm->pPrivateData );
      pFsm->pCurrentState = pFsm->pNewState;
   }
   pFsm->pCurrentState->_onDo( pFsm );
   if( pFsm->pCurrentState != pFsm->pNewState )
      pFsm->pCurrentState->_onExit( pFsm->pPrivateData );
}

#define FSM_TRANSITION( newState, attr... )                   \
   pFsm->pNewState = &state ## newState

#define FSM_DECLARE_STATE_XE( name, attr... )                 \
   struct STATE_T state ## name =                             \
   {                                                          \
      ._onEntry = name ## Entry,                              \
      ._onDo = name,                                          \
      ._onExit = name ## Exit                                 \
   }

#define FSM_INIT_FSM( fsm, privateData, startState, attr... ) \
   struct FSM_T fsm =                                         \
   {                                                          \
      .pNewState = &state ## startState,                      \
      .pCurrentState = NULL,                                  \
      .pPrivateData = privateData                             \
   }


struct STATE_T statePong;

void PingEntry( void* pPrivateData )
{
   *(int*)pPrivateData = 3;
   printf( "Function \"%s\"\n", __func__ );
}

void Ping( struct FSM_T* pFsm )
{
   int* pCounter = pFsm->pPrivateData;
   printf( "Function \"%s\"\n", __func__ );
   if( *pCounter == 0 )
   {
      FSM_TRANSITION( Pong, color=green, label='counter == 0' );
      return;
   }
   (*pCounter)--;

}

void PingExit( void* pPrivateData )
{
   printf( "Function \"%s\"\n", __func__ );
}

FSM_DECLARE_STATE_XE( Ping, color=red );

void PongEntry( void* pPrivateData )
{
   *(int*)pPrivateData = 3;
   printf( "Function \"%s\"\n", __func__ );
}

void Pong( struct FSM_T* pFsm )
{
   int* pCounter = pFsm->pPrivateData;
   printf( "Function \"%s\"\n", __func__ );
   if( *pCounter == 0 )
   {
      FSM_TRANSITION( Ping, color=red, label='counter == 0' );
      return;
   }
   (*pCounter)--;
}

void PongExit( void* pPrivateData )
{
   printf( "Function \"%s\"\n", __func__ );
}

FSM_DECLARE_STATE_XE( Pong );

int main( void )
{
   int counter;
   int i;

   FSM_INIT_FSM( myFsm, &counter, Ping, label='Start' );

   printf( "Test of %s\n", __FILE__ ); 

   for( i = 0; i < 10; i++ )
   {
      do
      {
         fsmExecute( &myFsm );
         sleep( 1 );
      }
      while( counter > 0 );
   }
   return 0;
}
