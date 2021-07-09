#ifndef MENU_H
#define MENU_H
#define PATH 100

#include "indici.h"

/*uscita dal programma liberando la lista di liste*/
void uscita(lista lista_auto, inddat indice_date, indsurn indice_cognomi);

/*banner admin*/
void banneradmin();

/*banner guest*/
void bannerguest();

void login_guest_menu();

/*menu dipendenti: visualizzare parco auto, vendere auto, inserire auto*/
void guest_menu(oste guest, oste head);

/*menu di ritiro delle auto: ritiro per targa/nome e cognome/data, oppure elimina tutte le auto in una data*/
void fifth_menu(lista lista_auto, inddat indice_date, indsurn indice_cognomi);

/*menu di inserimento auto: possibilità di inserire più auto insieme se nella data di inserimento non sono già presenti altre auto, inserimento in push delle auto in una data se sono già presenti auto nella stessa data, verifica che una targa sia già presente nel sistema*/
void forth_menu(lista lista_auto, inddat indice_date, indsurn indice_cognomi);

/*menu di gestione del parco auto: entrata al menu di inserimento o al menu di ritiro, visualizza lista date e auto annesse nel sistema*/
void third_menu(lista lista_auto, inddat indice_date, indsurn indice_cognomi);

/*menu gestione PIN dipendenti: visualizza PIN, modifica PIN*/
void menu_pin();

/*menu gestione cassa: visualizzare crediti temporanei acquisiti, visualizzare cassa fisica, prelievo da cassa*/
void gestione_cassa(lista lista_auto);

/*menu di accesso da amministratore: entrata in tutti i sottomenu, visualizzazione logs dei dipeendenti, visualizzazione auto in vendita, visualizzazione auto inserite dai guests*/
void second_menu();

/*primo menu di accesso da admin: accedi al sistema, reset della password*/
void first_menu(char save[15]);

#endif
