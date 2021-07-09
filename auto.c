#include "indici.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define PATH 100
#define READ 50


void freesinglenodolista(lista *head, char ritirata[10]){
  lista temp = *head, prev = NULL;

  if(strcmp((*head)->targa, ritirata) == 0){
    (*head) = (*head)->next;
    free(temp);
    return;
  }

  while (temp != NULL && strcmp(ritirata, temp->targa) > 0){
    prev = temp;
    temp = temp->next;
  }
  prev->next = temp->next;
  free(temp);

  return;
}

void freelista(lista head){
  lista tmp;

  while (head != NULL){
    tmp = head;
    head = head->next;
    free(tmp);
  }
}

void passtoautoinvendita(lista head){
  lista temp = head;
  sell testa = invendita();
  char brad[10], oggi[15];
  strcpy(brad, marca());
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);

  while(temp != NULL){
    if(controllodate(temp->index, oggi) == 1){
      insord_invendita(&testa, brad, temp->targa);
    }
    temp = temp->next;
  }

  if(testa != NULL){
    storevendite(testa);
    freevendite(testa);
  }
  return;
}

lista ricerca_targa(lista lista_auto, char plate[10]){
  lista temp = lista_auto;

  while(temp != NULL && strcmp(plate, temp->targa) > 0)
    temp = temp->next;
  if(strcmp(temp->targa, plate) == 0){
    printf("Targa trovata.\n");
    return temp;
  }
  if(temp == NULL || strcmp(plate, temp->targa) < 0){
    printf("Targa non trovata nel sistema.\n");
    return NULL;
  }
}

lista indice_targa(lista head, char plate[10]){
  lista temp = head;

  while(temp != NULL){
    if(strcmp(temp->targa, plate) == 0)
      return temp;
    temp = temp->next;
  }
}

int countall(lista head){
  lista temp = head;
  int counter = 0;
  while(temp != NULL){
    counter++;
    temp = temp->next;
  }
  return counter;
}

void print_auto(lista veic){
  char oggi[15];
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);
  if(controllodate(veic->index, oggi) == 0)
    printf("%s\t%s\t%s\t%c\n", veic->targa, veic->nome, veic->cognome, veic->log);
}

void print_lista(lista veic){
  while (veic != NULL) {
    print_auto(veic);
    veic = veic->next;
  }
}

void print_auto_dip(lista veic){
  char oggi[15];
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);
  if(controllodate(veic->index, oggi) == 0){
    if(veic->log == '*')
      printf("%s\t%s\t%s\t%c\n", veic->targa, veic->nome, veic->cognome, veic->log);
  }
}

void print_lista_dip(lista veic){
  while (veic != NULL) {
    print_auto_dip(veic);
    veic = veic->next;
  }
}

void printcarsinfile(lista head){
  FILE *fp = fopen("lista_auto.txt", "w");
  lista temp = head;

  if(fp == NULL){
    printf("Errore, file auto mancante.\n");
    exit(1);
  }

  while(temp != NULL){
    if(temp->next == NULL)
      fprintf(fp, "%s\t%s\t%s\t%s\t%c", temp->targa, temp->nome, temp->cognome, temp->index, temp->log);
    else
      fprintf(fp, "%s\t%s\t%s\t%s\t%c\n", temp->targa, temp->nome, temp->cognome, temp->index, temp->log);
    temp = temp->next;
  }
  fclose(fp);
}

int insord_newcar(lista* head_ref, char new_targa[10], char new_nome[15], char new_cognome[15], char data[15], int caller){
  lista temp, prev;
  lista new_car = (lista)malloc(sizeof(struct car));

  strcpy(new_car->targa, new_targa);
  strcpy(new_car->nome, new_nome);
  strcpy(new_car->cognome, new_cognome);
  strcpy(new_car->index, data);
  if(caller == 1)
    new_car->log = '*';
  if(caller == 0)
    new_car->log = ' ';

  if(*head_ref == NULL || strcmp(new_car->targa, (*head_ref)->targa) < 0){
    new_car->next = *head_ref;
    *head_ref = new_car;
    return 0;
  }

  temp = *head_ref;

  while(temp->next != NULL){
    if(strcmp(new_car->targa, temp->targa) == 0){
      printf("Targa già presente nel sistema. E' stata inserita il %s.\n", temp->index);
      return 1;
    }

    if(strcmp(new_car->targa, temp->targa) < 0){
      new_car->next = prev->next;
      prev->next = new_car;
      return 0;
    }
    prev = temp;
    temp = temp->next;
  }
  if(strcmp(new_car->targa, temp->targa) == 0){
    printf("Targa già presente nel sistema. E' stata inserita il %s.\n", temp->index);
    return 1;
  }
  if(strcmp(new_car->targa, temp->targa) < 0){
    new_car->next = prev->next;
    prev->next = new_car;
    return 0;
  }
  new_car->next = temp->next;
  temp->next = new_car;

  return 0;
}




sell invendita(){
  FILE *fp = fopen("Selling.txt", "r");
  sell head = NULL;

  char line[30], plate[10], brad[10];

  if(fp == NULL){
    printf("File vendite mancante! Uscita.\n");
    exit(1);
  }

  if(fgets(line, 30, fp) == NULL){
    printf("Nessuna auto e' in vendita al momento.\n");
    return NULL;
  }else{
    sscanf(line, "%s%s", plate, brad);
    insord_invendita(&head, brad, plate);


    while(fgets(line, 30, fp) != NULL){
      sscanf(line, "%s%s", plate, brad);
      insord_invendita(&head, brad, plate);
    }
    fclose(fp);
    return head;
  }
}

void freevendite(sell head){
  sell tmp;
  lista_vend temp, temp1;

  while (head != NULL){
    tmp = head;
    temp = tmp->sottolista;
    while(temp != NULL){
      temp1 = temp;
      temp = temp->next;
      free(temp1);
    }
    head = head->next;
    free(tmp);
  }
}

void freesingolobrand(sell *head, char brad[10]){
  sell temp = *head, prev = NULL;

  if(strcmp((*head)->brand, brad) == 0){
    (*head) = (*head)->next;
    free(temp);
    return;
  }

  while(temp != NULL && strcmp(temp->brand, brad) == 0){
    prev = temp;
    temp = temp->next;
  }
  prev->next = temp->next;
  free(temp);
}

int ricerca_vendita(lista_vend sottolista, char plate[10]){
  lista_vend tmp = sottolista;

  while(tmp != NULL){
    if(strcmp(tmp->targa, plate) == 0)
      return 0;
    tmp = tmp->next;
  }
  return 1;
}

void insord_sottolistainvendita(lista_vend *sottolista, char ordine[10]){
  lista_vend temp, prev, new_node;

  new_node = (lista_vend)malloc(sizeof(struct singvend));

  strcpy(new_node->targa, ordine);

  if(*sottolista == NULL || strcmp(new_node->targa, (*sottolista)->targa) < 0){
    new_node->next = *sottolista;
    *sottolista = new_node;
    return;
  }

  temp = *sottolista;

  while(temp->next != NULL){

    if(strcmp(new_node->targa, temp->targa) < 0){
      new_node->next = prev->next;
      prev->next = new_node;
      return;
    }
    prev = temp;
    temp = temp->next;
  }
  if(strcmp(new_node->targa, temp->targa) < 0){
    new_node->next = prev->next;
    prev->next = new_node;
    return;
  }

  new_node->next = temp->next;
  temp->next = new_node;

  return;
}

void insord_invendita(sell* head_ref, char brad[10], char plate[10]){
  sell temp, prev, new_node;

  if((*head_ref) == NULL || strcmp(brad, (*head_ref)->brand) < 0){
    new_node = (sell)malloc(sizeof(struct vendita));
    strcpy(new_node->brand, brad);
    new_node->sottolista = NULL;
    insord_sottolistainvendita(&(new_node->sottolista), plate);

    new_node->next = (*head_ref);
    (*head_ref) = new_node;
    return;
  }

  if(strcmp(brad, (*head_ref)->brand) == 0){
    insord_sottolistainvendita(&((*head_ref)->sottolista), plate);
    return;
  }

  temp = *head_ref;

  while(temp->next != NULL){
    if(strcmp(brad, temp->brand) == 0){
      insord_sottolistainvendita(&(temp->sottolista), plate);
      return;
    }

    if(strcmp(brad, temp->brand) < 0){
      new_node = (sell)malloc(sizeof(struct vendita));
      strcpy(new_node->brand, brad);
      new_node->sottolista = NULL;
      insord_sottolistainvendita(&(new_node->sottolista), plate);

      new_node->next = prev->next;
      prev->next = new_node;
      return;
    }
    prev = temp;
    temp = temp->next;
  }

  if(strcmp(brad, temp->brand) == 0){
    insord_sottolistainvendita(&(temp->sottolista), plate);
    return;
  }

  if(strcmp(brad, temp->brand) < 0){
    new_node = (sell)malloc(sizeof(struct vendita));
    strcpy(new_node->brand, brad);
    new_node->sottolista = NULL;
    insord_sottolistainvendita(&(new_node->sottolista), plate);

    new_node->next = prev->next;
    prev->next = new_node;
    return;
  }
  new_node = (sell)malloc(sizeof(struct vendita));
  strcpy(new_node->brand, brad);
  new_node->sottolista = NULL;
  insord_sottolistainvendita(&(new_node->sottolista), plate);

  new_node->next = temp->next;
  temp->next = new_node;
  return;
}

void print_singb(lista_vend sottolista){
  lista_vend tmp = sottolista;
  while(tmp != NULL){
    printf("%s\n", tmp->targa);
    tmp = tmp->next;
  }
}

void print_autoinvendita(sell head){
  sell temp = head;

  while(temp != NULL){
    printf("\n%s\n\n", temp->brand);
    print_singb(temp->sottolista);
    temp = temp->next;
  }
}

void storevendite(sell head){
  FILE *fp = fopen("Selling.txt", "w");
  sell temp = head;
  lista_vend tmp;

  if(fp == NULL){
    printf("File selling mancante.\n");
    exit(1);
  }

  while(temp != NULL){
    if(temp->next == NULL){
      tmp = temp->sottolista;
      while(tmp != NULL){
        if(tmp->next == NULL)
          fprintf(fp, "%s\t%s", tmp->targa, temp->brand);
        if(tmp->next != NULL)
          fprintf(fp, "%s\t%s\n", tmp->targa, temp->brand);
        tmp = tmp->next;
      }
    }
    if(temp->next != NULL){
      tmp = temp->sottolista;
      while(tmp != NULL){
        fprintf(fp, "%s\t%s\n", tmp->targa, temp->brand);
        tmp = tmp->next;
      }
    }
    temp = temp->next;
  }
  fclose(fp);
}

int brandsold(char in[10]){
  if(strcmp(in, "bmw") == 0)
    return 700;
  if(strcmp(in, "ford") == 0)
    return 400;
  if(strcmp(in, "audi") == 0)
    return 600;
  if(strcmp(in, "nissan") == 0)
    return 350;
  if(strcmp(in, "fiat") == 0)
    return 300;
  if(strcmp(in, "citroen") == 0)
    return 400;
  if(strcmp(in, "mercedes") == 0)
    return 600;
  if(strcmp(in, "jeep") == 0)
    return 500;
  if(strcmp(in, "porshe") == 0)
    return 850;
  if(strcmp(in, "tesla") == 0)
    return 1000;
}

char* marca(){
  srand(time(NULL));
  int r = rand()%10;

  if(r == 0){
    return "bmw";
  }
  if(r == 1){
    return "ford";
  }
  if(r == 2){
    return "audi";
  }
  if(r == 3){
    return "nissan";
  }
  if(r == 4){
    return "fiat";
  }
  if(r == 5){
    return "citroen";
  }
  if(r == 6){
    return "mercedes";
  }
  if(r == 7){
    return "jeep";
  }
  if(r == 8){
    return "porshe";
  }
  if(r == 9){
    return "tesla";
  }

}
