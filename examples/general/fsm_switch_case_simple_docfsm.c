#include <stdio.h>
#include <unistd.h>

#define FSM_TRANSITION( newStade, attr... ) state = newStade
#define FSM_INIT_FSM( startState, attr... ) state = startState
#define FSM_DECLARE_STATE( state, attr... ) state

typedef enum
{
   FSM_DECLARE_STATE( ON, color=red, label='Light is on' ),
   FSM_DECLARE_STATE( OFF, color=green )
} STATE_T;

int main( void )
{
   int i;
   STATE_T state;

   printf( "Test of %s\n", __FILE__ ); 
   
   FSM_INIT_FSM( ON, label='Start', color=blue );
   for( i = 0; i < 10; i++ )
   {
      switch( state )
      {
         case ON:
         {
            printf( "State: ON\n" );
            FSM_TRANSITION( OFF, color=red,
                                 label='On-time\nexpired' );
            break;
         }
         case OFF: {
            printf( "State: OFF\n" );
            FSM_TRANSITION( ON, color = green,
                                label = 'Off-time\n'
                                        'expired' );
            break;
         }
      }
      sleep( 1 );
   }
   return 0;
}

// gcc fsm_switch_case_simple_docfsm.c -o fsm
// docfsm fsm_switch_case_simple_docfsm.c | dot -Tpng -o myFsmSwitchCase_for_docfsm.png

