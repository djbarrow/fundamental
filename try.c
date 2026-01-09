#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int n[1];
  n[0]=atoi(argv[1])-2;
                //2+ (n[0] % (n[0] + (2 ^ n[0])))
  //printf("%d\n",2+ (n[0] % (n[0] + (2 ^ (2 - (2 ^ n[0]))))));
  printf("%d\n", 2+  (n[0] % (n[0] + (2 ^ (2 - (2 ^ n[0])))
			      + (2 ^ (2 ^ (2 - (2 ^ n[0])))))));
}
