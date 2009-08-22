#include <stdio.h>

#include "ffile.h"

int main(int argc, char *argv[])
{
   ffile f;

   f.create("test.file");


   f.close();


   return 0;
}
