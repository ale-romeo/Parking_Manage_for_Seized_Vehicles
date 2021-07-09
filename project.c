#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
  char log[15], pass[15], oggi[15];
  char* save;
  int count = 0;
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);


  system("clear");
  printf("/////////////////////////////////////////////\n");
  printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
  printf("/////////////////////////////////////////////\n");
  printf("DATA: %s\n\n\n\n", ctime(&now));

  printf("Vuoi effettuare il log-in come DIPENDENTE o come ADMIN?\n");
  scanf("%s", log);

  while((strcmp(log, "admin") != 0 && strcmp(log, "ADMIN") != 0 && strcmp(log, "DIPENDENTE") != 0 && strcmp(log, "dipendente") != 0) && count < 3){
    printf("Inserimento errato. Riprova, dipendente o admin?\n");
    scanf(" %s", log);
    count++;
  }
  if(count == 3){
    printf("Tentativi esauriti.\n");
    return 1;
  }

  if(strcmp(log, "admin") == 0 || strcmp(log, "ADMIN") == 0){
    save = verify_pass();
    strcpy(pass, save);
    free(save);
    system("clear");
    printf("/////////////////////////////////////////////\n");
    printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
    printf("                     ADMIN\n");
    printf("/////////////////////////////////////////////\n");
    printf("DATA: %s\n\n\n\n", ctime(&now));
    if(login(pass) == 0)
      first_menu(pass);
    else if(login(pass) == 1)
      return 1;

  }else if(strcmp(log, "DIPENDENTE") == 0 || strcmp(log, "dipendente") == 0){
    system("clear");
    printf("/////////////////////////////////////////////\n");
    printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
    printf("                     DIPENDENTE\n");
    printf("/////////////////////////////////////////////\n");
    printf("DATA: %s\n\n\n\n", ctime(&now));


    login_guest_menu();
  }
  return 0;
}
