#include <stdio.h>
#include <unistd.h>

// gcc myFsmFuncPtr_for_docfsm.c -o myFsmFuncPtr_for_docfsm

#define FSM_DECLARE_STATE( state, attr... ) void state ( void** ppState )
#define FSM_TRANSITION( newState, attr... ) *ppState = newState
#define FSM_INIT_FSM( startState, attr... ) currentstate = startState

typedef void (*STATE_F)(void**); 
 
FSM_DECLARE_STATE( on, color=green );
FSM_DECLARE_STATE( off, color=red );

void on( void** ppState )
{
   printf( "State: \"on\"\n" );
   FSM_TRANSITION( off, color=red );
}

void off( void** ppState )
{
   printf( "State: \"off\"\n" );
   FSM_TRANSITION( on,color=green );
}

int main( void )
{
   int i;
   STATE_F currentstate;
   FSM_INIT_FSM( off, color=red, label='Start' );
   
   printf( "Test of %s\n", __FILE__ ); 

   for( i = 0; i < 10; i++ )
   {
      currentstate( (void**)&currentstate );
      sleep( 1 );
   }
   return 0;
}
