#include <stdio.h>

#include "farchive.h"

int main(int /*argc*/, char */*argv*/[])
{
   farchive f;

   f.create("test.arc");
   f.close();


   return 0;
}
