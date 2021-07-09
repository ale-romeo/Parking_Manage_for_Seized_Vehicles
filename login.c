#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

PIN initpin(){
  FILE *fp = fopen("PIN.txt", "r");

   if(fp == NULL){
    int pindip, m;

    printf("Nessun PIN utente impostato. Inserirne un PIN di MASSIMO 4 cifre:\n");
    scanf(" %d", &pindip);
    while(pindip >= 10000 && m < 3){
      printf("Il PIN deve essere di MASSIMO 4 cifre. Riprova.\n");
      scanf(" %d", &pindip);
      m++;
    }
    if(m == 3){
      printf("Tentativi esauriti. Riprova piu' tardi.\n");
      exit(1);
    }
    fp = fopen("Pin.txt", "w");
    fprintf(fp, "%d\t%d", pindip, pindip);
    fclose(fp);

    PIN codice = (PIN)malloc(sizeof(struct mypin));
    codice->PIN = pindip;
    codice->prev = pindip;
  }

  PIN codice = (PIN)malloc(sizeof(struct mypin));
  fscanf(fp, "%d%d", &(codice->PIN), &(codice->prev));
  fclose(fp);

  return codice;
}

void resetPIN(PIN *codice){
  int pass, pin, c = 0, k = 0, m = 0;

  printf("Inserire vecchio PIN:\t");
  scanf(" %d", &pass);

  while(pass != (*codice)->PIN && c < 2){
    printf("\nPIN errato. Riprova.\n");
    printf("Inserire vecchio PIN:\t");
    scanf(" %d", &pass);
    c++;
  }
  if(c == 2){
    printf("Tentativi esauriti.\n");
    return;
  }
  c = 0;

  if(pass == (*codice)->PIN){
    do{
      printf("\nInserisci nuovo PIN:\t");
      scanf(" %d", &pin);
      while(pin >= 10000 && k < 3){
        printf("\nIl PIN deve essere di MASSIMO 4 cifre. Riprova.\n");
        scanf(" %d", &pin);
        m++;
      }
      if(k == 3){
        printf("Tentativi esauriti. Riprova piu' tardi.\n");
        exit(1);
      }

      if(pin == (*codice)->prev || pin == (*codice)->PIN){
        printf("\nIl nuovo PIN deve essere diverso da quelli impostati di recente.\n");
        c++;
      }
    }while((pin == (*codice)->prev || pin == (*codice)->PIN)  && c < 3);

    if(c == 3){
      printf("Tentativi esauriti.\n");
      return;
    }

    (*codice)->prev = (*codice)->PIN;
    (*codice)->PIN = pin;
    printf("\nNuovo PIN registrato correttamente.\n");
  }
}

void storePIN(PIN codice){
  FILE *fp = fopen("PIN.txt", "w");

  fprintf(fp, "%d\t%d", codice->PIN, codice->prev);
  fclose(fp);
}

char* verify_pass(){
  char name[15] = "Chiavi.txt", boot_pass[15];
  FILE *pass = fopen(name, "r");
  int exist = 0, c = 0;
  char *save;

  save = malloc(15*sizeof(char));

  if(pass == NULL){
    printf("Password admin compromessa.\n");
    exit(1);
  }

  /*verifica se il file che contiene la password è vuoto*/
  c = fgetc(pass);
  if(c == EOF)
    exist = 1;
  fclose(pass);
  /*verifica se il file che contiene la password è vuoto*/

  /*read or write file*/
  if(exist == 0){
    pass = fopen(name, "r");
    fgets(save, 15, pass);
    fclose(pass);
  }else{
    pass = fopen(name, "w");
    printf("Primo avvio del Programma di gestione parco mezzi sequestrati\nImpostare nuova password di massimo 10 caratteri:\n");
    scanf("%s", boot_pass);
    while(strlen(boot_pass) > 15) {
      printf("La password deve contenere MASSIMO 10 caratteri!\n Inserisci la password correttamente:\n");
      scanf("%s", boot_pass);

    }

    fprintf(pass, "%s", boot_pass);
    fclose(pass);
    pass = fopen(name, "r");
    fgets(save, 15, pass);
    fclose(pass);
  }

  return save;
}

int login(char saved[15]){
  char password[15];
  int  counter = 0;
  time_t now;
  time(&now);

  printf("Inserire password Amministratore:\n");
  scanf(" %s", password);
  system("clear");

  while((strcmp(saved, password) != 0) && counter < 2){
    printf("/////////////////////////////////////////////\n");
    printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
    printf("                     ADMIN\n");
    printf("/////////////////////////////////////////////\n");
    printf("DATA: %s\n\n\n\n", ctime(&now));
    printf("La password non corrisponde. Riprova\nInserire password:\n");
    scanf(" %s", password);
    counter++;
    system("clear");
  }

  if(strcmp(saved, password) == 0){
    return 0;
  }else{
    printf("/////////////////////////////////////////////\n");
    printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
    printf("                     ADMIN\n");
    printf("/////////////////////////////////////////////\n");
    printf("DATA: %s\n\n\n\n", ctime(&now));
    printf("Tentativi esauriti, riprovare piu' tardi\n");
    exit(1);
  }
}

void reset_pass(char saved[15]){
  FILE *pass;
  char nomefile[15] = "Chiavi.txt", oldpass[15], newpass[15], two[15];
  int counter = 0;
  time_t now;
  time(&now);

  printf("Inserire vecchia password:\n");
  scanf("%s", oldpass);

  while(strcmp(saved, oldpass) != 0 && counter < 2){
    printf("Password errata. Riprova:\n");
    scanf("%s", oldpass);
    counter++;
  }
  if(counter == 2){
    printf("Tentativi esauriti, riprovare piu' tardi\n");
    exit(1);
  }

  counter = 0;

  if(strcmp(saved, oldpass) == 0){
    printf("Inserire nuova password:\n");
    scanf("%s", newpass);
    printf("Inserire nuovamente la nuova password:\n");
    scanf("%s", two);


    while(strcmp(newpass, two) != 0 && counter < 2) {
      printf("Le password non corrispondono! Riprova:\n");
      system("clear");
      printf("Inserire nuova password:\n");
      scanf("%s", newpass);
      printf("Inserire nuovamente la nuova password:\n");
      scanf("%s", two);
      counter++;
    }
    if(strcmp(newpass, two) != 0){
      printf("Le password non coincidono. Riprova piu' tardi.\n");
      exit(1);
    }

    pass = fopen(nomefile, "w");
    fprintf(pass, "%s", newpass);
    fclose(pass);

    printf("Password impostata correttamente!\n");
  }else{
    printf("Tentativi esauriti, riprovare piu' tardi.\n");
    exit(1);
  }
}

void printinguest(oste head){
  FILE *fp;
  oste temp = head;
  fp = fopen("Guest.txt", "w");
  while(temp != NULL){

    fprintf(fp, "Log effettuato il:\t%s\n", temp->log);
    if(temp->next == NULL)
      fprintf(fp, "%s\t%s\t%d\t%d\t%d", temp->nguest, temp->surguest, temp->ins, temp->view, temp->sold);
    else
      fprintf(fp, "%s\t%s\t%d\t%d\t%d\n", temp->nguest, temp->surguest, temp->ins, temp->view, temp->sold);

    temp = temp->next;
  }

  fclose(fp);
}

oste read_guest(){
  FILE *fp = fopen("Guest.txt", "r");
  if(fp == NULL){
    return NULL;
  }

  oste head = NULL, curr = NULL;
  char dati[50], log[40], dump[20], oggi[15];
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;
  sprintf(oggi, "%d.%d.%d", td, tm, ty);

  head = (oste)malloc(sizeof(struct utente));
  fgets(log, 40, fp);
  sscanf(log, "%s%s%s%s", dump, dump, dump, head->log);
  fgets(dati, 50, fp);
  sscanf(dati, "%s%s%d%d%d", head->nguest, head->surguest, &head->ins, &head->view, &head->sold);

  curr = head;

  while(fgets(log, 40, fp) != NULL){
    curr->next = (oste)malloc(sizeof(struct utente));
    curr = curr->next;

    sscanf(log, "%s%s%s%s", dump, dump, dump, curr->log);
    fgets(dati, 50, fp);
    sscanf(dati, "%s%s%d%d%d", curr->nguest, curr->surguest, &curr->ins, &curr->view, &curr->sold);
  }
  curr->next = NULL;
  fclose(fp);
  return head;
}

void controllolog(oste one){
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);
  oste temp = one, temp1 = one;
  int count = 0, td, tm, ty;
  char oggi[15];

  td = local->tm_mday;
  tm = local->tm_mon + 1;
  ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);

  while(temp != NULL){
    if(strcmp(temp->log, oggi) == 0)
      count++;
    temp = temp->next;
  }

  printf("Sono stati effettuati %d accessi al sistema dai dipendenti.\n", count);
  sleep(5);

  if(count > 0){
    printf("Ecco la lista dei dipendenti e le loro attivita':\n\n");

    while(temp1 != NULL){
      if(strcmp(temp1->log, oggi) == 0){
        printf("%s\t%s\n", temp1->nguest, temp1->surguest);
        if(temp1->ins != 0)
          printf("Ha effettuato un nuovo inserimento %d volte oggi.\n", temp1->ins);
        if(temp1->view != 0)
          printf("Ha visualizzato il parco auto %d volte oggi.\n", temp1->view);
        if(temp1->sold != 0)
          printf("Ha venduto %d auto oggi.\n", temp1->sold);
        if(temp1->view == 0 && temp1->ins == 0 && temp1->sold == 0)
          printf("Nessuna attivita' svolta oggi.\n");
      }
      temp1 = temp1->next;
    }
  }
}

int check_log(oste head, char name[15], char surname[15]){
  oste temp = head;
  while(temp != NULL){
    if((strcmp(name, head->nguest) == 0) && (strcmp(surname, head->surguest) == 0)){
      printf("Guest gia' esistente nel sistema.\n");
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}

void signup(oste *head, char name[15], char surname[15], char oggi[15]){
  int pin, op = 0, pin1;
  oste new_guest = (oste)malloc(sizeof(struct utente));

  strcpy(new_guest->nguest, name);
  strcpy(new_guest->surguest, surname);
  strcpy(new_guest->log, oggi);

  new_guest->ins = 0;
  new_guest->view = 0;
  new_guest->sold = 0;

  do{
    if(op != 0)
      printf("I PIN non corrispondono!\n");
    printf("\nInserire pin fornito dall'amministratore:\t");
    scanf(" %d", &pin);

    printf("\nReinserire pin per conferma:\t");
    scanf(" %d", &pin1);
    op++;
  }while((pin != pin1) && (op < 3));

  if(op == 3){
    printf("Tentativi esauriti. Uscita...\n");
    sleep(2);
    freeguest(*head);
    exit(1);
  }

  new_guest->next = (*head);
  (*head) = new_guest;

  return;
}

void freeguest(oste head){
  oste curr;
  while(head != NULL){
    curr = head;
    head = head->next;
    free(curr);
  }
}
