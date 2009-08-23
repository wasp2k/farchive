#include <stdio.h>

#include "farchive.h"

char buf[512];

int main(int /*argc*/, char */*argv*/[])
{
   farchive f;

   f.create("test.arc");


   f.close();


   return 0;
}
