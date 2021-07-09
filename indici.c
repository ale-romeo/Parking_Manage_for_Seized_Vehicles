#include "indici.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#define PATH 100



lista getlistafromfile(inddat* indice_date, indsurn* indice_cognomi){
  FILE *fp1 = fopen("lista_auto.txt", "r");
  lista temp, head = NULL;
  char line[50], dump;

  if(fp1 == NULL){
    printf("File store delle auto inesistente!");
    exit(1);
  }

  if(fgets(line, 50, fp1) != NULL){
    head = (lista)malloc(sizeof(struct car));

    sscanf(line, "%s%s%s%s%c%c", head->targa, head->nome, head->cognome, head->index, &dump, &head->log);
    insord_data(indice_date, head);
    insord_surn(indice_cognomi, head);
  }

  temp = head;

  while(fgets(line, 50, fp1) != NULL){
    temp->next = (lista)malloc(sizeof(struct car));
    temp = temp->next;

    sscanf(line, "%s%s%s%s%c%c", temp->targa, temp->nome, temp->cognome, temp->index, &dump, &temp->log);
    insord_data(indice_date, temp);
    insord_surn(indice_cognomi, temp);
  }
  temp->next = NULL;

  fclose(fp1);

  passtoautoinvendita(head);
  return head;
}

indsurn init_indicecognomi(lista lista_auto){
  lista temp = lista_auto;
  indsurn tmp = NULL;

  while(temp != NULL){
    insord_surn(&tmp, temp);
    temp = temp->next;
  }
  return tmp;
}

inddat init_indicedate(lista lista_auto){
  lista temp = lista_auto;
  inddat tmp = NULL;

  while(temp != NULL){
    insord_data(&tmp, temp);
    temp = temp->next;
  }
  return tmp;
}

void freedate(inddat head){
  inddat tmp;
  lista temp, temp1;

  while (head != NULL){
    tmp = head;
    temp = tmp->sottolista;
    while (temp != NULL){
      temp1 = temp;
      temp = temp->next;
      free(temp1);
    }
    head = head->next;
    free(tmp);
  }
}

void freecognomi(indsurn head){
  indsurn tmp;
  lista temp, temp1;

  while (head != NULL){
    tmp = head;
    temp = tmp->sottolista;
    while (temp != NULL){
      temp1 = temp;
      temp = temp->next;
      free(temp1);
    }
    head = head->next;
    free(tmp);
  }
}

void freenodo_sottolistasurn(lista *sottolista, char surname[15], char plate[10]){
  lista temp = *sottolista, prev = NULL;

  if(strcmp((*sottolista)->cognome, surname) == 0 && strcmp(plate, (*sottolista)->targa) == 0){
    (*sottolista) = (*sottolista)->next;
    free(temp);
    return;
  }

  while(strcmp(surname, temp->cognome) > 0){
    prev = temp;
    temp = temp->next;
  }
  while(strcmp(temp->cognome, surname) == 0 && strcmp(plate, temp->targa) != 0){
    prev = temp;
    temp = temp->next;
  }
  prev->next = temp->next;
  free(temp);

  return;
}

void freenodo_sottolistadate(lista *sottolista, char plate[10]){
  lista temp = *sottolista, prev = NULL;

  if(strcmp(plate, (*sottolista)->targa) == 0){
    (*sottolista) = (*sottolista)->next;
    free(temp);
    return;
  }

  while(strcmp(plate, temp->targa) > 0){
    prev = temp;
    temp = temp->next;
  }
  prev->next = temp->next;
  free(temp);

  return;
}

inddat ricerca_date(inddat indice_date, char data[15]){
  inddat temp = indice_date;

  while(cfrdates(data, temp->date) == 2)
    temp = temp->next;
  if(cfrdates(data, temp->date) == 1){
    printf("Non ci sono auto inserite in questa data.\n");
    return NULL;
  }

  if(cfrdates(data, temp->date) == 0)
    return temp;
}

indsurn ricerca_lettera(indsurn indice_cognomi, char surname[15]){
  indsurn temp = indice_cognomi;

  while(surname[0] > temp->lettera)
    temp = temp->next;
  if(surname[0] < temp->lettera){
    printf("Proprietario inesistente.\n");
    return NULL;
  }
  if(surname[0] == temp->lettera)
    return temp;
}

lista ricerca_cognome(lista sottolista, char surname[15]){
  lista tmp = sottolista, prev, temp = sottolista;
  int count = 0;
  char name[15];

  while(tmp != NULL && strcmp(surname, tmp->cognome) >= 0){
    if(strcmp(surname, tmp->cognome) == 0){
      count++;
      prev = tmp;
    }
    tmp = tmp->next;
  }

  if(count == 1){
    printf("%s %s e' il proprietario dell'auto targata %s.\n", prev->nome, prev->cognome, prev->targa);
    return prev;
  }

  if(count > 1){
    printf("Riscontrata omonimia.\n Inserire nome proprietario:\n");
    scanf(" %s", name);

    while(temp != NULL && strcmp(surname, temp->cognome) >= 0){
      if(strcmp(temp->nome, name) == 0 && strcmp(temp->cognome, surname) == 0){
        printf("%s %s e' il proprietario dell'auto targata %s.\n", temp->nome, temp->cognome, temp->targa);
        return temp;
      }
      temp = temp->next;
    }
    count = 0;
  }

  if(count == 0){
    printf("Nessuna corrispondenza nel sistema.\n");
    return NULL;
  }

}

void print_data(inddat data){
  printf("%s\n", data->date);
}

void print_inddat(inddat data){
  while(data != NULL){
    print_data(data);
    data = data->next;
  }
}

void printauto_percognome(indsurn indice_cognomi){
  printf("I veicoli contrassegnati da * sono stati inseriti dai dipendenti.\n");
  while(indice_cognomi != NULL){
    printf("\n\n%c\n\n", toupper(indice_cognomi->lettera));
    print_lista(indice_cognomi->sottolista);
    indice_cognomi = indice_cognomi->next;
  }
}

void printauto_perdata(inddat indice_date){
  printf("I veicoli contrassegnati da * sono stati inseriti dai dipendenti.\n");
  while(indice_date != NULL){
    printf("\n\n%s\n\n", indice_date->date);
    print_lista(indice_date->sottolista);
    indice_date = indice_date->next;
  }
}

void ins_newcar(char data[15], lista* veic, inddat* indice_date, indsurn* indice_cognomi, int caller){
  char plate[10], name[15], surname[15];
  lista nodolista;
  int c = 0;

  do{
    printf("Immettere numero di targa del veicolo da inserire:\n");
    scanf(" %s", plate);
    c++;
  }while(verificatarga(plate) == 1 && c < 3);
  if(c == 3){
    printf("Tentativi esauriti.\n");
    return;
  }

  printf("Immettere nome proprietario:\n");
  scanf("%s", name);
  printf("Immettere cognome proprietario:\n");
  scanf("%s", surname);
  if(insord_newcar(veic, plate, name, surname, data, caller) == 1)
    return;

  nodolista = indice_targa((*veic), plate);

  insord_data(indice_date, nodolista);

  insord_surn(indice_cognomi, nodolista);

  printcarsinfile((*veic));
}

void insord_sottolistadate(lista *sottolista, lista nodolista){
  lista temp, prev, new_node = (lista)malloc(sizeof(struct car));

  strcpy(new_node->targa, nodolista->targa);
  strcpy(new_node->nome, nodolista->nome);
  strcpy(new_node->cognome, nodolista->cognome);
  strcpy(new_node->index, nodolista->index);
  new_node->log = nodolista->log;

  if(*sottolista == NULL || strcmp(new_node->targa, (*sottolista)->targa) < 0){
    new_node->next = *sottolista;
    *sottolista = new_node;
    return;
  }

  temp = *sottolista;

  while(temp->next != NULL){
    if(strcmp(new_node->targa, (*sottolista)->targa) < 0){
      new_node->next = prev->next;
      prev->next = new_node;
      return;
    }
    prev = temp;
    temp = temp->next;
  }

  new_node->next = temp->next;
  temp->next = new_node;

  return;
}

void insord_data(inddat* head_ref, lista nodolista){
  inddat temp, prev = NULL, new_node;

  char dgy[15], tempdate[15];


  if(*head_ref == NULL){
    new_node = (inddat)malloc(sizeof(struct data));
    strcpy(new_node->date, nodolista->index);
    new_node->sottolista = NULL;
    insord_sottolistadate(&(new_node->sottolista), nodolista);
    new_node->next = *head_ref;
    *head_ref = new_node;
    return;
  }

  strcpy(dgy, nodolista->index);

  strcpy(tempdate, (*head_ref)->date);

  if(cfrdates(dgy, tempdate) == 0){
    insord_sottolistadate(&((*head_ref)->sottolista), nodolista);
    return;
  }

  if(cfrdates(dgy, tempdate) == 1){
    new_node = (inddat)malloc(sizeof(struct data));
    strcpy(new_node->date, nodolista->index);
    new_node->sottolista = NULL;
    insord_sottolistadate(&(new_node->sottolista), nodolista);
    new_node->next = *head_ref;
    *head_ref = new_node;
    return;
  }
  temp = *head_ref;

  while(temp->next != NULL){
    strcpy(tempdate, temp->date);

    if(cfrdates(dgy, tempdate) == 0){
      insord_sottolistadate(&(temp->sottolista), nodolista);
      return;
    }

    if(cfrdates(dgy, tempdate) == 1){
      new_node = (inddat)malloc(sizeof(struct data));
      strcpy(new_node->date, nodolista->index);
      new_node->sottolista = NULL;
      insord_sottolistadate(&(new_node->sottolista), nodolista);
      new_node->next = prev->next;
      prev->next = new_node;
      return;
    }
    prev = temp;
    temp = temp->next;
  }

  strcpy(tempdate, temp->date);

  if(cfrdates(dgy, tempdate) == 0){
    insord_sottolistadate(&(temp->sottolista), nodolista);
    return;
  }
  if(cfrdates(dgy, tempdate) == 1){
    new_node = (inddat)malloc(sizeof(struct data));
    strcpy(new_node->date, nodolista->index);
    new_node->sottolista = NULL;
    insord_sottolistadate(&(new_node->sottolista), nodolista);

    new_node->next = temp->next;
    temp->next = new_node;
    return;
  }
  if(cfrdates(dgy, tempdate) == 2){
  new_node = (inddat)malloc(sizeof(struct data));
  strcpy(new_node->date, nodolista->index);
  new_node->sottolista = NULL;
  insord_sottolistadate(&(new_node->sottolista), nodolista);

  new_node->next = temp->next;
  temp->next = new_node;
  return;
  }
}

void insord_sottolistacognomi(lista* sottolista, lista nodolista){
  lista temp, prev, new_node;

  new_node = (lista)malloc(sizeof(struct car));

  strcpy(new_node->targa, nodolista->targa);
  strcpy(new_node->nome, nodolista->nome);
  strcpy(new_node->cognome, nodolista->cognome);
  strcpy(new_node->index, nodolista->index);
  new_node->log = nodolista->log;

  if(*sottolista == NULL || strcmp(new_node->cognome, (*sottolista)->cognome) <= 0){
    new_node->next = *sottolista;
    *sottolista = new_node;
    return;
  }

  temp = *sottolista;

  while(temp->next != NULL){

    if(strcmp(new_node->cognome, temp->cognome) <= 0){
      new_node->next = prev->next;
      prev->next = new_node;

      return;
    }
    prev = temp;
    temp = temp->next;
  }

  new_node->next = temp->next;
  temp->next = new_node;

  return;
}

void insord_surn(indsurn* head_ref, lista nodolista){
  indsurn temp, prev = NULL, new_node;


  if(*head_ref == NULL || nodolista->cognome[0] < (*head_ref)->lettera){
    new_node = (indsurn)malloc(sizeof(struct surn));
    new_node->lettera = nodolista->cognome[0];
    new_node->sottolista = NULL;
    insord_sottolistacognomi(&(new_node->sottolista), nodolista);

    new_node->next = *head_ref;
    *head_ref = new_node;
    return;
  }

  if(nodolista->cognome[0] == (*head_ref)->lettera){
    insord_sottolistacognomi(&((*head_ref)->sottolista), nodolista);
    return;
  }

  temp = *head_ref;

  while(temp->next != NULL){
    if(nodolista->cognome[0] == temp->lettera){
      insord_sottolistacognomi(&(temp->sottolista), nodolista);
      return;
    }

    if(nodolista->cognome[0] < temp->lettera){
      new_node = (indsurn)malloc(sizeof(struct surn));
      new_node->lettera = nodolista->cognome[0];
      new_node->sottolista = NULL;
      insord_sottolistacognomi(&(new_node->sottolista), nodolista);

      new_node->next = prev->next;
      prev->next = new_node;

      return;
    }
    prev = temp;
    temp = temp->next;
  }

  if(nodolista->cognome[0] == temp->lettera){
    insord_sottolistacognomi(&(temp->sottolista), nodolista);
    return;
  }
  if(nodolista->cognome[0] < temp->lettera){
    new_node = (indsurn)malloc(sizeof(struct surn));
    new_node->lettera = nodolista->cognome[0];
    new_node->sottolista = NULL;
    insord_sottolistacognomi(&(new_node->sottolista), nodolista);

    new_node->next = prev->next;
    prev->next = new_node;

    return;
  }
  new_node = (indsurn)malloc(sizeof(struct surn));
  new_node->lettera = nodolista->cognome[0];
  new_node->sottolista = NULL;
  insord_sottolistacognomi(&(new_node->sottolista), nodolista);

  new_node->next = temp->next;
  temp->next = new_node;
}

void ritiroplate(lista* lista_auto, inddat* indice_date, indsurn *indice_cognomi, cassa* fondi){
  int m = 0, virt = 0, i = 0;
  char c, plate[10], surname[15];

  lista temp = *lista_auto;
  indsurn tmp1;
  inddat tmp2;

  printf("Inserire numero di targa del veicolo:\n");
  scanf("%s", plate);

  while((temp = ricerca_targa(*lista_auto, plate)) == NULL && m < 3){
    printf("Ritenta.\n");
    printf("Inserire numero di targa del veicolo:\n");
    scanf("%s", plate);
    m++;
  }
  if(m == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }

  printf("Ritirare il veicolo?\nY o N\n");
  scanf(" %c", &c);

  while((c != 'N') && (c != 'n') && (c != 'Y') && (c != 'y') && (i < 3)){
    printf("Carattere errato. Riprova.\n");
    scanf(" %c", &c);
    i++;
  }
  if(i == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }

  if(c == 'N' || c == 'n'){
    printf("Arrivederci.\n");
    return;
  }

  virt = credstocash(temp->index);

  tmp1 = ricerca_lettera(*indice_cognomi, temp->cognome);

  tmp2 = ricerca_date(*indice_date, temp->index);
  strcpy(surname, temp->cognome);

  freesinglenodolista(lista_auto, temp->targa);

  freenodo_sottolistasurn(&(tmp1->sottolista), surname, plate);

  freenodo_sottolistadate(&(tmp2->sottolista), plate);

  printf("Auto ritirata correttamente!\n");

  add_cassa(fondi, virt);
  sleep(3);

  printf("Cassa aggiornata!\n");

  printcarsinfile(*lista_auto);
}

void ritironame(lista *lista_auto, inddat* indice_date, indsurn *indice_cognomi, cassa *fondi){
  int m = 0, i = 0, virt = 0;
  char d, surname[15], plate[10];

  indsurn temp;
  inddat tmp2;
  lista tmp, tmp1;

  printf("Inserire cognome del proprietario:\n");
  scanf(" %s", surname);

  while((temp = ricerca_lettera(*indice_cognomi, surname)) == NULL && m < 3){
    printf("Ritenta.\n");
    printf("Inserire cognome del proprietario:\n");
    scanf(" %s", surname);
    m++;
  }
  if(m == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }
  m = 0;

  while((tmp = ricerca_cognome(temp->sottolista, surname)) == NULL && m < 3){
    printf("Ritenta.\n");
    printf("Inserire cognome del proprietario:\n");
    scanf(" %s", surname);
    m++;
  }

  if(m == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }

  printf("Vuoi ritirare questo veicolo?\nY o N(continua ricerca in altre date)\n");
  scanf(" %c", &d);

  while((d != 'N') && (d != 'n') && (d != 'Y') && (d != 'y') && (i < 3)){
    printf("Carattere errato. Riprova.\n");
    scanf(" %c", &d);
    i++;
  }
  if(i == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }

  while(d == 'N' || d == 'n'){
    tmp1 = tmp;
    if((tmp = ricerca_cognome(tmp->next, surname)) == NULL){
      printf("Non ci sono altri proprietari con questo cognome.\n");
      printf("Vuoi ritirare l'ultimo veicolo trovato?\nY o N(continua ricerca in altre date)\n");
      scanf(" %c", &d);
      tmp = tmp1;
      if(d == 'N' || d == 'n')
        return;
    }
    printf("Vuoi ritirare questo veicolo?\nY o N(continua ricerca in altre date)\n");
    scanf(" %c", &d);
  }

  virt = credstocash(tmp->index);
  strcpy(plate, tmp->targa);

  tmp2 = ricerca_date(*indice_date, tmp->index);

  freesinglenodolista(lista_auto, tmp->targa);

  freenodo_sottolistasurn(&(temp->sottolista), surname, plate);

  freenodo_sottolistadate(&(tmp2->sottolista), plate);


  printf("Auto ritirata correttamente!\n");

  add_cassa(fondi, virt);
  sleep(3);

  printf("Cassa aggiornata!\n");

  printcarsinfile(*lista_auto);
}

void ritirobydate(lista *lista_auto, inddat *indice_date, indsurn* indice_cognomi, cassa *fondi){
  int virt = 0, m = 0;
  char date[15], plate[10], surname[15];

  indsurn tmp1;
  inddat temp;
  lista tmp;

  printf("Inserire data da ricercare:\n");
  scanf(" %s", date);

  while((temp = ricerca_date(*indice_date, date)) == NULL && m < 3){
    printf("Ritenta.\n");
    printf("Inserire data da ricercare:\n");
    scanf(" %s", date);
    m++;
  }
  if(m == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }
  m = 0;

  printf("Ecco la lista delle auto inserite il %s:\n", date);
  print_lista(temp->sottolista);

  printf("Inserire numero di targa dell'auto da rimuovere:\n");
  scanf("%s", plate);

  while((tmp = ricerca_targa(temp->sottolista, plate)) == NULL && m < 3){
    printf("Ritenta.\n");
    printf("Inserire numero di targa dell'auto da rimuovere:\n");
    scanf("%s", plate);
    m++;
  }
  if(m == 3){
    printf("Tentativi esauriti. Riprova piu' tardi.\n");
    return;
  }

  virt = credstocash(tmp->index);

  tmp1 = ricerca_lettera(*indice_cognomi, tmp->cognome);
  strcpy(surname, tmp->cognome);

  freesinglenodolista(lista_auto, tmp->targa);

  freenodo_sottolistasurn(&(tmp1->sottolista), surname, plate);

  freenodo_sottolistadate(&(temp->sottolista), plate);


  printf("Auto ritirata correttamente!\n");

  add_cassa(fondi, virt);
  sleep(3);

  printf("Cassa aggiornata!\n");

  printcarsinfile(*lista_auto);
}

int cfrdates(char data[15], char cfr[15]){
  char *day, *month, *year, *dayz, *monthz, *yearz;
  int dd, mm, yy, d1, m1, y1;
  char foo[15], fool[15];
  strcpy(foo, data);

  day = strtok(foo, ".");
  dd = atoi(day);
  month = strtok(NULL, ".");
  mm = atoi(month);
  year = strtok(NULL, ".");
  yy = atoi(year);

  mm = mm*30;
  yy = yy*365;
  int insdate = dd+mm+yy;

  strcpy(fool, cfr);

  dayz = strtok(fool, ".");
  d1 = atoi(dayz);
  monthz = strtok(NULL, ".");
  m1 = atoi(monthz);
  yearz = strtok(NULL, ".");
  y1 = atoi(yearz);

  m1 = m1*30;
  y1 = y1*365;
  int cfrdate = d1+m1+y1;

  if(insdate > cfrdate)
    return 1;
  if(insdate == cfrdate)
    return 0;
  if(insdate < cfrdate)
    return 2;
}

int controllodate(char data[15], char cfr[15]){
  char *day, *month, *year, *dayz, *monthz, *yearz;
  int dd, mm, yy, d1, m1, y1;
  char foo[15], fool[15];
  strcpy(foo, data);

  day = strtok(foo, ".");
  dd = atoi(day);
  month = strtok(NULL, ".");
  mm = atoi(month);
  year = strtok(NULL, ".");
  yy = atoi(year);

  if(dd > 31 || mm > 12 || yy > 2021){
    printf("Errore nell'inserimento della data.\n");
    return 2;
  }
  mm = mm*30;
  yy = yy*365;
  int insdate = dd+mm+yy;

  strcpy(fool, cfr);

  dayz = strtok(fool, ".");
  d1 = atoi(dayz);
  monthz = strtok(NULL, ".");
  m1 = atoi(monthz);
  yearz = strtok(NULL, ".");
  y1 = atoi(yearz);

  m1 = m1*30;
  y1 = y1*365;
  int cfrdate = d1+m1+y1;

  if((cfrdate-insdate) > 1825 || insdate > cfrdate)
    return 1;
  else if((cfrdate-insdate) < 1825)
    return 0;
}

int verificatarga(char plate[10]){
  int len = strlen(plate), i = 0;
  if(len > 7 || len < 7){
    printf("Errore nell'inserimento del numero di targa. Deve essere una targa Italiana.\n");
    return 1;
  }

  if(len == 7){
    while(i < len){
      if(i < 2 || i > 4){
        if(plate[i] < 65 || (plate[i] > 90 && plate[i] < 97) || plate[i] > 122){
          printf("Errore nell'inserimento del numero di targa. Deve essere una targa Italiana.\n");
          return 1;
        }
      }
      if(i > 1 && i < 5){
        if(plate[i] < 48 || plate[i] > 57){
          printf("Errore nell'inserimento del numero di targa. Deve essere una targa Italiana.\n");
          return 1;
        }
      }

      i++;
    }
  }
  return 0;
}
