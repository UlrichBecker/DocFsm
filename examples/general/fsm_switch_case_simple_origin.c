#include <stdio.h>
#include <unistd.h>

typedef enum
{
   ON,
   OFF
} STATE_T;

int main( void )
{
   int i = 0;
   STATE_T state = OFF;

   printf( "Test of %s\n", __FILE__ ); 

   for( i = 0; i < 10; i++ )
   {
      switch( state )
      {
         case ON:
            printf( "State: ON\n" );
            state = OFF;
            break;
         case OFF:
            printf( "State: OFF\n" );
            state = ON;
            break;
      }
      sleep( 1 );
   }
   return 0;
}

// gcc fsm_switch_case_simple_origin.c -o fsm
