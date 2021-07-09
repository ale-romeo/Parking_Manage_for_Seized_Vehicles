#ifndef AUTO_H
#define AUTO_H

typedef struct car* lista;

struct car{
  char targa[10];
  char nome[15];
  char cognome[15];
  char index[15];
  char log;
  lista next;
};

typedef struct singvend* lista_vend;

struct singvend{
  char targa[10];
  lista_vend next;
};

typedef struct vendita* sell;

struct vendita{
  char brand[10];
  lista_vend sottolista;
  sell next;
};


/*libera singolo nodo dalla lista delle auto*/
void freesinglenodolista(lista *head, char ritirata[10]);

/*libera lista delle auto*/
void freelista(lista head);

//passa alla lista delle auto in vendita le auto che nella lista delle auto hanno superato i 5 anni
void passtoautoinvendita(lista head);

/*ricerca per targa nella lista delle auto*/
lista ricerca_targa(lista lista_auto, char plate[10]);

//restituisce puntatore al nodo che contiene la targa passata
lista indice_targa(lista head, char plate[10]);

//conteggio nodi lista delle auto
int countall(lista head);

/*stampa auto*/
void print_auto(lista veic);

/*stampa lista auto*/
void print_lista(lista veic);

/*stampa auto dipendenti*/
void print_auto_dip(lista veic);

/*stampa lista auto dipendenti*/
void print_lista_dip(lista veic);

/*stampa lista auto di una data in file restituendo errore se il file non esiste*/
void printcarsinfile(lista head);

/*inserimento in testa tramite deferenziazione*/
int insord_newcar(lista* head_ref, char new_targa[10], char new_nome[15], char new_cognome[15], char data[15], int caller);



/*crea lista auto in vendita da file*/
sell invendita();

/*libera lista dei brand delle auto in vendita e sottolista di auto in vendita*/
void freevendite(sell head);

void freesingolobrand(sell *head, char brad[10]);

//ricerca per targa nella lista delle auto in vendita
int ricerca_vendita(lista_vend sottolista, char plate[10]);

//inserimento ordinato per targa nodo in sottolista delle auto in vendita
void insord_sottolistainvendita(lista_vend *sottolista, char ordine[10]);

/*inserimento ordinato per brand di nuovo nodo auto in vendita*/
void insord_invendita(sell* head_ref, char brad[10], char plate[10]);

//stampa singolo nodo dell'indice delle marche
void print_singb(lista_vend sottolista);

/*stampa le auto in vendita*/
void print_autoinvendita(sell head);

/*stampa auto in vendita in file*/
void storevendite(sell head);

//restituisce valore monetario della marca di auto passata
int brandsold(char in[10]);

//crea valore randomico per assegnare la marca all'auto che passa dalla lista delle auto alla sottolista delle auto in vendita
char* marca();


#endif
