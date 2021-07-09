#ifndef CASSA_H
#define CASSA_H
#include "auto.h"


typedef struct nodesto* storico;

struct nodesto{
  char targa[10];
  char data[15];
  storico next;
};

typedef struct mycassa* cassa;

struct mycassa{
  int fisica;
  int virtuale;
  storico chrono;
};


//inizializza e popola cassa
cassa initcassa(lista lista_auto);

//libera cassa e la lista delle auto vendute
void freecassa(cassa fondi);

/*restituisce credito virtuale di una singola auto*/
int credstocash(char data[15]);

/*restituisce credito virtuale di una data lista di auto*/
int crediti(lista head);

//inizializza e popola lista delle auto vendute
storico lista_autovendute();

/*restituisce il valore salvato della cassa fisica*/
int cassafisica();

/*aggiunge denaro a fisica nel tipo di dato cassa*/
void add_cassa(cassa* fondi, int virt);

/*sottrae denaro da fisica nel tipo di dato cassa*/
void prelievo(cassa* head);

void freenodo_sottolista(lista_vend *sottolista, char plate[10]);

/*richiama freenodo_sottolista ed eventualmente libera nodo sell del brand*/
void vendita_auto(sell *head, cassa *fondi);

void storecassa(cassa fondi);

//salva storico date in file
void storestorico(storico lista_vendute);

//passa le informazioni dal nodo della lista delle auto in vendita, crea nuovo nodo nella lista delle auto vendute e lo inserisce in testa(ordinato per data di vendita)
void passtostorico(storico *head_ref, char oggi[15], char plate[15]);

//stampa a video la lista delle auto vendute
void printstorico(storico lista_autovendute);

#endif
