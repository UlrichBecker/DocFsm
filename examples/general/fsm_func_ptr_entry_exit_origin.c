#include <stdio.h>
#include <unistd.h>

// gcc fsm_func_ptr_entry_exit_origin.c -o fsm

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

struct STATE_T statePong;

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
      pFsm->pNewState = &statePong;
      return;
   }
   (*pCounter)--;

}

void PingExit( void* pPrivateData )
{
   printf( "Function \"%s\"\n", __func__ );
}

struct STATE_T statePing =
{
   ._onEntry = PingEntry,
   ._onDo = Ping,
   ._onExit = PingExit
};

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
      pFsm->pNewState = &statePing;
      return;
   }
   (*pCounter)--;
}

void PongExit( void* pPrivateData )
{
   printf( "Function \"%s\"\n", __func__ );
}

struct STATE_T statePong =
{
   ._onEntry = PongEntry,
   ._onDo = Pong,
   ._onExit = PongExit
};

int main( void )
{
   int counter;
   int i;

   struct FSM_T myFsm =
   {
      .pNewState = &statePing,
      .pCurrentState = NULL,
      .pPrivateData = &counter
   };

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
