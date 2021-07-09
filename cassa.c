#include "indici.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


cassa initcassa(lista lista_auto){
  cassa head = (cassa)malloc(sizeof(struct mycassa));

  head->fisica = cassafisica();
  head->virtuale = crediti(lista_auto);
  head->chrono = lista_autovendute();

  return head;
}

void freecassa(cassa fondi){
  storico tmp;
  while(fondi->chrono != NULL){
    tmp = fondi->chrono;
    fondi->chrono = fondi->chrono->next;
    free(tmp);
  }
  free(fondi);
}

int credstocash(char data[15]){
  int td, tm, ty, tday, day, month, year, insdate, credito = 0;
  char *daycar, *monthcar, *yearcar, tempdate[15];
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);
  td = local->tm_mday;
  tm = local->tm_mon + 1;
  ty = local->tm_year + 1900;

  tm = tm*30;
  ty = ty*365;
  tday = td+tm+ty;

  strcpy(tempdate, data);
  daycar = strtok(tempdate, ".");
  day = atoi(daycar);
  monthcar = strtok(NULL, ".");
  month = atoi(monthcar);
  yearcar = strtok(NULL, ".");
  year = atoi(yearcar);

  month = month*30;
  year = year*365;
  insdate = day+month+year;

  if(insdate > (tday-90))
    credito+= 0;
  if(insdate < (tday-90) && insdate > (tday-180))
    credito+= 400;
  if(insdate < (tday-180) && insdate > (tday-365))
    credito+= 700;
  if(insdate < (tday-365) && insdate > (tday-730))
    credito+= 1000;
  if(insdate < (tday-730) && insdate > (tday-1095))
    credito+= 900;
  if(insdate < (tday-1095) && insdate > (tday-1460))
    credito+= 800;
  if(insdate < (tday-1460) && insdate >= (tday-1825))
    credito+= 700;

  return credito;
}

int crediti(lista head){
  int day, month, year, credito = 0, insdate, td, tm, ty, tday;
  char *daycar, *monthcar, *yearcar, tempdate[15];
  time_t now;
  time(&now);

  struct tm *local = localtime(&now);
  lista temp = head;

  td = local->tm_mday;
  tm = local->tm_mon + 1;
  ty = local->tm_year + 1900;

  tm = tm*30;
  ty = ty*365;
  tday = td+tm+ty;

  while(temp != NULL){

    strcpy(tempdate, temp->index);
    daycar = strtok(tempdate, ".");
    day = atoi(daycar);
    monthcar = strtok(NULL, ".");
    month = atoi(monthcar);
    yearcar = strtok(NULL, ".");
    year = atoi(yearcar);

    month = month*30;
    year = year*365;
    insdate = day+month+year;

    if(insdate > (tday-90))
      credito+= 0;
    if(insdate < (tday-90) && insdate > (tday-180))
      credito+= 400;
    if(insdate < (tday-180) && insdate > (tday-365))
      credito+= 700;
    if(insdate < (tday-365) && insdate > (tday-730))
      credito+= 1000;
    if(insdate < (tday-730) && insdate > (tday-1095))
      credito+= 900;
    if(insdate < (tday-1095) && insdate > (tday-1460))
      credito+= 800;
    if(insdate < (tday-1460) && insdate >= (tday-1825))
      credito+= 700;
    if(insdate < (tday-1825))
      credito+= 0;

    temp = temp->next;
  }
  return credito;
}

storico lista_autovendute(){
  FILE *fp = fopen("History.txt", "r");

  if(fp == NULL){
    printf("File storico vendite mancante.\n");
    exit(1);
  }

  char line[30];
  storico head = NULL, temp;

  if(fgets(line, 30, fp) != NULL){
    head = (storico)malloc(sizeof(struct nodesto));
    sscanf(line, "%s%s", head->targa, head->data);
  }

  temp = head;

  while(fgets(line, 30, fp) != NULL){
    temp->next = (storico)malloc(sizeof(struct nodesto));
    temp = temp->next;
    sscanf(line, "%s%s", temp->targa, temp->data);
  }
  temp->next = NULL;
  fclose(fp);
  return head;
}

int cassafisica(){
  FILE *fp = fopen("Cassa.txt", "r");
  int denaro;

  if(fp == NULL){
    printf("Cassa non disponibile! Uscita.\n");
    exit(1);
  }

  fscanf(fp, "%d", &denaro);
  fclose(fp);

  return denaro;
}

void add_cassa(cassa* fondi, int virt){
  (*fondi)->fisica += virt;
  storecassa(*fondi);
}

void prelievo(cassa *head){
  int prel, current = (*head)->fisica;

  printf("Quanto denaro si desidera prelevare?\n");
  scanf(" %d", &prel);

  if(prel > current){
    printf("Fondi insufficienti per il prelievo.\n");
    return;
  }

  (*head)->fisica = current-prel;

  storecassa(*head);
}

void freenodo_sottolista(lista_vend *sottolista, char plate[10]){
  lista_vend temp = *sottolista, prev = NULL;

  if(strcmp((*sottolista)->targa, plate) == 0){
    (*sottolista) = (*sottolista)->next;
    free(temp);
    return;
  }

  while(temp != NULL && strcmp(plate, temp->targa) > 0){
    prev = temp;
    temp = temp->next;
  }
  prev->next = temp->next;
  free(temp);

  return;
}

void vendita_auto(sell *head, cassa *fondi){
  sell tmp = *head, prev;
  char brad[10], plate[10], oggi[15], toup;
  int m = 0, i = 0;

  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;
  sprintf(oggi, "%d.%d.%d", td, tm, ty);

  printf("Inserire MARCA di auto, per la quale visualizzare la lista delle auto in vendita:\n");
  scanf(" %s", brad);

  while(tmp != NULL && strcmp(brad, tmp->brand) > 0){
    prev = tmp;
    tmp = tmp->next;
  }
  if(tmp == NULL || strcmp(brad, tmp->brand) < 0){
    printf("Non è in vendita nessuna auto %s\n", brad);
    return;
  }

  printf("\n\n");
  while(brad[i]){
    toup = brad[i];
    printf("%c", toupper(toup));
    i++;
  }
  printf("\n\n");
  print_singb(tmp->sottolista);

  printf("Quale delle auto si desidera vendere?\nInserire numero di targa:\t");
  scanf(" %s", plate);

  while(ricerca_vendita(tmp->sottolista, plate) == 1 && m < 3){
    printf("\nLa targa non corrisponde.\nRiprova:\t");
    scanf(" %s", plate);
    m++;
  }
  if(m == 3){
    printf("Tentativi esauriti. Uscita.\n");
    return;
  }

  add_cassa(fondi, brandsold(tmp->brand));
  passtostorico(&((*fondi)->chrono), oggi, plate);
  freenodo_sottolista(&(tmp->sottolista), plate);
  if(tmp->sottolista == NULL){
    prev->next = tmp->next;
    free(tmp);
  }
  printf("Auto venduta!\n");

  storevendite(*head);
  return;
}

void storecassa(cassa fondi){
  FILE *fp = fopen("Cassa.txt", "w");
  fprintf(fp, "%d", fondi->fisica);
  fclose(fp);
}

void storestorico(storico lista_vendute){
  FILE *fp = fopen ("History.txt", "w");
  if(fp == NULL){
    printf("File history mancante.\n");
    exit(1);
  }
  storico temp;

  temp = lista_vendute;

  while(temp != NULL){
    if(temp->next == NULL)
      fprintf(fp, "%s\t%s", temp->targa, temp->data);
    if(temp->next != NULL)
      fprintf(fp, "%s\t%s\n", temp->targa, temp->data);
    temp = temp->next;
  }
  fclose(fp);
}

void passtostorico(storico *head_ref, char oggi[15], char plate[15]){
  storico nodo = (storico)malloc(sizeof(struct nodesto));
  strcpy(nodo->targa, plate);
  strcpy(nodo->data, oggi);
  nodo->next = *head_ref;
  *head_ref = nodo;

  storestorico(*head_ref);
  return;
}

void printstorico(storico lista_autovendute){
  storico temp = lista_autovendute;
  while(temp != NULL){
    printf("%s\tVenduta in data:%s\n", temp->targa, temp->data);
    temp = temp->next;
  }
  return;
}
