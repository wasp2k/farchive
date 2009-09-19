#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "farchive.h"

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */

void usage(void)
{
   printf( "farc v0.1\n"
           "\n"
           "  farc [cmd] [archive] [files]\n"
           "\n"
           "  l     list archive\n"
           "\n" );
}

/* ------------------------------------------------------------------------- */

void list(const char *fname)
{
   farchive f;

   if ( f.open(fname) == -1 )
   {
      printf( "failed to open archive\n" );
   } else
   {
      if ( f.moveFirst() == -1 )
      {
      } else
      {
         do
         {
            const fobject &obj = f.getCurrObj();
            printf( "Obj: %8d Chain: %8d Type: %3d Opt: 0x%04x Ofs: %8d Size: %d\n", obj.getId(), obj.getNextChain(), obj.getType(), obj.getOptions(), obj.getOfs(), obj.getSize() );
         } while( f.moveNext() != -1 );
      }
      f.close();
   }
}

/* ------------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
   if ( ( argc >= 2) && !strcmp( argv[1], "l" ) )
   {
      list(argv[2]);
   } else
   {
      usage();
   }
}
