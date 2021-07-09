#ifndef LOGIN_H
#define LOGIN_H
#include <stdio.h>
#include <time.h>

typedef struct mypin* PIN;

struct mypin{
  int PIN;
  int prev;
};

typedef struct utente* oste;

struct utente{
  char nguest[15];
  char surguest[15];
  char log[15];
  int ins;
  int view;
  int sold;
  oste next;
};

//inizializza struct PIN tramite lettura da file
PIN initpin();

//salva struct PIN in file
void storePIN(PIN codice);

//reset PIN dipendenti da parte dell'admin
void resetPIN(PIN *codice);

/*dato un file di salvataggio della password, verifica che non sia vuoto, nel caso lo sia fa inserire una nuova password (primo avvio)*/
char* verify_pass();

/*verifica che la password che viene inserita sia corretta*/
int login(char saved[15]);

/*fornisce la possibilità da parte dell'admin di resettare la password*/
void reset_pass(char saved[15]);

/*stampa lista logs guest in file*/
void printinguest(oste head);

/*legge file di login guest*/
oste read_guest();

/*controllo logs dei guests*/
void controllolog(oste one);

/*check se l'utente è già esistente*/
int check_log(oste head, char name[15], char surname[15]);

/*inserisce nuovo guest*/
void signup(oste *head, char name[15], char surname[15], char oggi[15]);

/*elimina lista log*/
void freeguest(oste head);


#endif
