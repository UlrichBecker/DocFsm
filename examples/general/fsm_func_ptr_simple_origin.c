#include <stdio.h>
#include <unistd.h>

// gcc fsm_func_ptr_simple_origin.c -o fsm

typedef void (*STATE_F)(void**);
 
void on( void** ppState );
void off( void** ppState );

void on( void** ppState )
{
   printf( "State: \"on\"\n" );
   *ppState = off;
}

void off( void** ppState )
{
   printf( "State: \"off\"\n" );
   *ppState = on;
}

int main( void )
{
   int i;
   STATE_F currentstate = off;

   printf( "Test of %s\n", __FILE__ ); 

   for( i = 0; i < 10; i++ )
   {
      currentstate( (void**)&currentstate );
      sleep( 1 );
   }
   return 0;
}
