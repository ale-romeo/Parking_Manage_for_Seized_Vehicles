#ifndef INDICI_H
#define INDICI_H
#define PATH 100

#include "auto.h"
#include "login.h"
#include "cassa.h"


typedef struct surn* indsurn;

struct surn{
  char lettera;
  lista sottolista;
  indsurn next;
};

typedef struct data* inddat;

struct data{
  char date[15];
  lista sottolista;
  inddat next;
};



//inizializza e popola lista delle auto da file, popola indici per cognome e per data
lista getlistafromfile(inddat* indice_date, indsurn* indice_cognomi);

//inizializza e popola indice cognomi da lista delle auto
indsurn init_indicecognomi(lista lista_auto);

//iniizializza e popola indice date da lista delle auto
inddat init_indicedate(lista lista_auto);

//libera indice date e sottoliste
void freedate(inddat head);

//libera indice cognomi e sottoliste
void freecognomi(indsurn head);

void freenodo_sottolistasurn(lista *sottolista, char surname[15], char plate[10]);

void freenodo_sottolistadate(lista *sottolista, char plate[10]);

inddat ricerca_date(inddat indice_date, char data[15]);

//ricerca iniziale cognome da indice dei cognomi
indsurn ricerca_lettera(indsurn indice_cognomi, char surname[15]);

/*ricerca cognome nella sottolista dell'indice corrispondente*/
lista ricerca_cognome(lista sottolista, char surname[15]);

//restituisce puntatore al nodo dell'indice delle date che contiene la data passata
inddat indice_data(inddat today, char data[15]);

/*stampa data singola*/
void print_data(inddat data);

/*stampa tutte le date da indice delle date*/
void print_inddat(inddat data);

//stampa le auto inidicizzate per cognome
void printauto_percognome(indsurn indice_cognomi);

/*stampa le auto indicizzate per data*/
void printauto_perdata(inddat indice_date);

//inserimento ordinato per targa del nodo nella lista delle auto, richiamando le funzioni di inserimento ordinato per gli indici
void ins_newcar(char data[15], lista* veic, inddat* indice_date, indsurn* indice_cognomi, int caller);

//inserimento ordinato per data nella sottolista dell'indice delle date
void insord_sottolistadate(lista *sottolista, lista nodolista);

//inserimento ordinato nuovo nodo nell'indice delle date, se tale indice esiste già, allora procede semplicemente all'inserimento del nuovo nodo nella sottolista delle auto ordinate per date
void insord_data(inddat* head_ref, lista nodolista);

//inserimento ordinato per cognome nella sottolista dell'indice cognomi
void insord_sottolistacognomi(lista* sottolista, lista nodolista);

//inserimento ordinato nuovo nodo nell'indice dei cognomi, se tale indice esiste già, allora procede semplicemente all'inserimento del nuovo nodo nella sottolista delle auto ordinate per cognome
void insord_surn(indsurn* head_ref, lista nodolista);

//effettua ricerca per targa nella lista delle auto, libera nodo dalla lista delle auto e aggiunge denaro fisico alla cassa in fisica
void ritiroplate(lista* lista_auto, inddat* indice_date, indsurn *indice_cognomi, cassa* fondi);

//effettua ricerca per cognome nella sottolista dell'indice per cognome, libera nodo dalla lista delle auto e aggiunge denaro fisico alla cassa in fisica(sottraendo crediti virtuali da virtuale)
void ritironame(lista* lista_auto, inddat* indice_date, indsurn *indice_cognomi, cassa* fondi);

//effettua ricerca per data nella sottolista dell'indice per data, libera nodo dalla lista delle auto e aggiunge denaro fisico alla cassa in fisica(sottraendo crediti virtuali da virtuale)
void ritirobydate(lista *lista_auto, inddat *indice_date, indsurn* indice_cognomi, cassa *fondi);

//confronta due date restituendo 3 valori distinti per i casi di maggiore, minore o uguale
int cfrdates(char data[15], char cfr[15]);

//controlla che le date di inserimento siano corrette
int controllodate(char data[15], char cfr[15]);

//verifica l'inserimento di un numero di targa corretto
int verificatarga(char plate[10]);

#endif
