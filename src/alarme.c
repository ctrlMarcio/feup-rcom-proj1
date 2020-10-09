#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int flag = 1, count = 1;

void atende() // atende alarme
{
   printf("alarme # %d\n", count);
   flag = 1;
   count++;
}

main()
{

   (void)signal(SIGALRM, atende); // instala  rotina que atende interrupcao

   while (count < 4)
   {
      if (flag)
      {
         alarm(3); // activa alarme de 3s
         flag = 0;
      }
   }
   printf("Vou terminar.\n");
}
