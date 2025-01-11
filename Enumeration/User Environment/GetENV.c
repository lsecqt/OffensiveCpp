#include <stdio.h>

int main () {
   printf("UserName : %s\n", getenv("USERNAME"));
   printf("Path : %s\n", getenv("PATH"));
   printf("UserHome : %s\n", getenv("USERPROFILE"));

   return(0);
}