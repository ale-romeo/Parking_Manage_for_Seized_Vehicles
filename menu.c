#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define PATH 100

void uscita(lista lista_auto, inddat indice_date, indsurn indice_cognomi){
  freelista(lista_auto);
  freedate(indice_date);
  freecognomi(indice_cognomi);
  exit(1);
}

void banneradmin(){
  time_t now;
  time(&now);

  system("clear");
  printf("/////////////////////////////////////////////\n");
  printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
  printf("                     ADMIN\n");
  printf("/////////////////////////////////////////////\n");
  printf("DATA: %s\n\n\n\n", ctime(&now));
}

void bannerguest(){
  time_t now;
  time(&now);

  system("clear");
  printf("/////////////////////////////////////////////\n");
  printf("     GESTORE DEL PARCO MEZZI SEQUESTRATI\n");
  printf("                      DIPENDENTE\n");
  printf("/////////////////////////////////////////////\n");
  printf("DATA: %s\n\n\n\n", ctime(&now));
}

void login_guest_menu(){
  oste lista_dipendenti = read_guest(), temp;
  int check, cv, pin, op = 0, pin1, goon = 0;
  char name[15], surname[15], oggi[15];
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  int td = local->tm_mday;
  int tm = local->tm_mon + 1;
  int ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);
  PIN pinadmin = initpin();


  printf("Come si desidera procedere?\n1)Signup\n2)Login\n3)Esci\n");
  scanf(" %d", &cv);
  switch(cv){
    case 1:
      bannerguest();

      printf("Inserire credenziali nuovo guest:\nInserire nome:\t");
      scanf(" %s", name);
      printf("\nInserire cognome:\t");
      scanf(" %s", surname);
      printf("\n");

      check = check_log(lista_dipendenti, name, surname);
      if(check == 0){
        signup(&lista_dipendenti, name, surname, oggi);
        printinguest(lista_dipendenti);

        bannerguest();
        freeguest(lista_dipendenti);
        free(pinadmin);
        login_guest_menu();
      }
      if(check == 1){
        printf("Errore! Esiste già un utente con queste credenziali. Uscita...\n");
        freeguest(lista_dipendenti);
        free(pinadmin);
        exit(1);
      }
    break;

    case 2:
      bannerguest();

      printf("Inserire credenziali dipendente:\nInserire nome:\t");
      scanf(" %s", name);
      printf("\nInserire cognome:\t");
      scanf(" %s", surname);
      printf("\n");

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
        freeguest(lista_dipendenti);
        exit(1);
      }

      temp = lista_dipendenti;

      while(temp != NULL){
        if((strcmp(name, temp->nguest) == 0) && (strcmp(surname, temp->surguest) == 0)){
          if(pinadmin->PIN == pin){
            printf("Accesso effettuato come:\t%s\t%s\n", temp->nguest, temp->surguest);
            if(strcmp(temp->log, oggi) != 0){
              printf("Ultimo log-in effettuato in data:\t%s\n", temp->log);
              printf("Aggiorno log alla data di oggi...\n");
              sleep(3);
              strcpy(temp->log, oggi);
              temp->ins = 0;
              temp->view = 0;
              temp->sold = 0;
            }

            sleep(2);
            free(pinadmin);
            guest_menu(temp, lista_dipendenti);
          }

          if(pinadmin->PIN != pin){
            printf("PIN errato. Uscita...\n");
            sleep(1);
            free(pinadmin);
            freeguest(lista_dipendenti);
            exit(1);
          }
        }
        temp = temp->next;
      }
      if(temp == NULL){
        printf("Nessun login è stato effettuato con queste credenziali!\n");
        printf("Premere INVIO per tornare indietro...\n");
        getchar();
        while(goon != '\n')
          goon = getchar();

        bannerguest();
        free(pinadmin);
        freeguest(lista_dipendenti);
        login_guest_menu();
      }

    break;

    case 3:
      printf("Arrivederci.\n");
      free(pinadmin);
      freeguest(lista_dipendenti);
      exit(1);
    break;
  }
}

void guest_menu(oste guest, oste head){
  int h, z = 0, keep, td, tm, ty, flag = 1, goon = 0;
  char put, oggi[15];

  oste temp0;
  cassa fondi;
  inddat indice_date = NULL;
  indsurn indice_cognomi = NULL;
  lista lista_auto = getlistafromfile(&indice_date, &indice_cognomi);

  sell top;

  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  td = local->tm_mday;
  tm = local->tm_mon + 1;
  ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);

  bannerguest();

  printf("Come si desidera procedere?\n1)Visualizzare tutte le auto presenti nel sistema\n2)Inserire nuovo veicolo\n3)Vendere un'auto\n4)Esci\n");
  scanf(" %d", &h);

  bannerguest();

  switch(h){
    case 1:
      printf("Ecco tutte le auto presenti nel sistema fino ad oggi:\n");
      print_lista(lista_auto);
      guest->view++;

      temp0 = head;
      while(temp0 != NULL && temp0 != guest)
        temp0 = temp0->next;

      if(temp0 == NULL)
        printinguest(guest);
      if(temp0 != NULL)
        printinguest(head);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      bannerguest();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          bannerguest();
          freedate(indice_date);
          freecognomi(indice_cognomi);
          freelista(lista_auto);

          guest_menu(guest, head);
        break;

        case 2:
          printf("Arrivederci.\n");
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;
    case 2:
      printf("Come dipendente, puoi inserire i veicoli solo nella data di oggi. Procedere con l'inserimento di una nuova vettura? Y o N\n");
      scanf(" %c", &put);

      bannerguest();

      while((put != 'n' && put != 'N' && put  != 'Y' && put != 'y') && z < 3){
        printf("Carattere errato. Riprova.\n");
        scanf(" %c", &put);
        z++;
      }
      if(z == 2){
        printf("Tentativi esauriti. Riprova piu' tardi.\n");

        uscita(lista_auto, indice_date, indice_cognomi);
      }

      if(put == 'n' || put == 'N'){
        bannerguest();
        freedate(indice_date);
        freecognomi(indice_cognomi);
        freelista(lista_auto);

        guest_menu(guest, head);
      }
      if(put == 'y' || put == 'Y'){
        ins_newcar(oggi, &lista_auto, &indice_date, &indice_cognomi, flag);
        printf("Auto inserita correttamente!\n");
        guest->ins++;
        printinguest(head);
      }

      sleep(4);

      bannerguest();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          bannerguest();
          freedate(indice_date);
          freecognomi(indice_cognomi);
          freelista(lista_auto);

          guest_menu(guest, head);
        break;

        case 2:
          printf("Arrivederci.\n");
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 3:
      top = invendita();
      fondi = initcassa(lista_auto);

      vendita_auto(&top, &fondi);

      guest->sold++;
      getchar();
      getchar();

      bannerguest();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          bannerguest();
          freecassa(fondi);
          freevendite(top);
          freedate(indice_date);
          freecognomi(indice_cognomi);
          freelista(lista_auto);

          guest_menu(guest, head);
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(fondi);
          freevendite(top);
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          free(fondi);
          freevendite(top);
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 4:
      printf("Arrivederci.\n");
      freeguest(head);

      uscita(lista_auto, indice_date, indice_cognomi);
    break;

    default:
      printf("Arrivederci.\n");
      freeguest(head);

      uscita(lista_auto, indice_date, indice_cognomi);
    break;
  }
}

void fifth_menu(lista lista_auto, inddat indice_date, indsurn indice_cognomi){
  int e, keep;

  printf("Come si desidera procedere?\n1)Ricerca e rimuovi veicolo da ritirare per targa\n2)Ricerca e rimuovi veicolo da ritirare per cognome\n3)Ricerca e rimuovi veicolo per data di inserimento\n4)Torna indietro\n5)Esci\n");
  scanf(" %d", &e);

  banneradmin();

  cassa fondi = initcassa(lista_auto);

  switch(e){
    case 1:
      ritiroplate(&lista_auto, &indice_date, &indice_cognomi, &fondi);

      sleep(5);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(fondi);

          fifth_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 2:
      ritironame(&lista_auto, &indice_date, &indice_cognomi, &fondi);

      sleep(5);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(fondi);

          fifth_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 3:
      ritirobydate(&lista_auto, &indice_date, &indice_cognomi, &fondi);

      sleep(5);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(fondi);

          fifth_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 4:
      banneradmin();
      freecassa(fondi);

      third_menu(lista_auto, indice_date, indice_cognomi);
    break;

    case 5:
      printf("Arrivederci\n");
      freecassa(fondi);

      uscita(lista_auto, indice_date, indice_cognomi);
    break;

    default:
      printf("Arrivederci.\n");
      freecassa(fondi);

      uscita(lista_auto, indice_date, indice_cognomi);
    break;
  }
}

void forth_menu(lista lista_auto, inddat indice_date, indsurn indice_cognomi){
  char data[15], oggi[15], surname[15], plate[10];
  int b, c, d, q, td, tm, ty, keep, flag = 0, goon = 0;
  indsurn right;
  inddat right1;

  time_t now;
  time(&now);
  struct tm *local = localtime(&now);

  td = local->tm_mday;
  tm = local->tm_mon + 1;
  ty = local->tm_year + 1900;

  sprintf(oggi, "%d.%d.%d", td, tm, ty);

  printf("Scegliere un'opzione:\n1)Inserisci nuovo/i veicolo/i\n2)Ricercare un veicolo\n3)Visualizza parco auto\n4)Torna indietro\n5)Esci\n");
  scanf(" %d", &c);

  banneradmin();

  switch(c){
    case 1:
      printf("Si desidera inserire..\n1)Nella data di oggi\n2)In un'altra data\n");
      scanf(" %d", &q);

      switch(q){
        case 1:
          ins_newcar(oggi, &lista_auto, &indice_date, &indice_cognomi, flag);

        break;

        case 2:
          printf("Inserire data(gg.mm.aaaa):\n");
          scanf(" %s", data);

          while(controllodate(data, lista_auto->index) == 2){
            sleep(2);
            printf("Inserire data(gg.mm.aaaa):\n");
            scanf(" %s", data);
          }

          if(controllodate(data, oggi) == 1){
            printf("La data di inserimento puo' essere passata di non piu' di 5 anni\n");
            sleep(5);

            banneradmin();

            printf("1.Torna indietro\n2.Esci\n");
            scanf(" %d", &keep);
            switch(keep){
              case 1:
                banneradmin();

                forth_menu(lista_auto, indice_date, indice_cognomi);
              break;

              case 2:
                printf("Arrivederci.\n");

                uscita(lista_auto, indice_date, indice_cognomi);
              break;

              default:
                printf("Arrivederci.\n");

                uscita(lista_auto, indice_date, indice_cognomi);
              break;
            }
          }
          else if(controllodate(data, oggi) == 0){
            ins_newcar(data, &lista_auto, &indice_date, &indice_cognomi, flag);
          }

        break;
      }
      printf("Auto inserita correttamente!\n");

      sleep(5);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();

          forth_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;
    case 2:
      printf("1)Ricerca per targa\n2)Ricerca per cognome\n3)Ricerca per data\n");
      scanf(" %d", &d);
      switch(d){
        case 1:
          printf("Inserire targa da ricercare:\t");
          scanf(" %s", plate);
          printf("\n");

          ricerca_targa(lista_auto, plate);
        break;
        case 2:
          printf("Inserire cognome da cercare:\t");
          scanf(" %s", surname);
          printf("\n");
          right = ricerca_lettera(indice_cognomi, surname);
          if(right != NULL)
            ricerca_cognome(right->sottolista, surname);
        break;
        case 3:
          printf("Inserire data da ricercare:\t");
          scanf(" %s", data);
          printf("\n");
          right1 = ricerca_date(indice_date, data);
          if(right1 != NULL){
            printf("Ecco la lista delle auto inserite il %s:\n", data);
            print_lista(right1->sottolista);
            sleep(2);
            printf("Inserire targa da ricercare:\t");
            scanf(" %s", plate);
            printf("\n");
            ricerca_targa(right1->sottolista, plate);
          }
        break;
      }
      sleep(5);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();

          forth_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;
    case 3:
      printf("Come si desidera ordinare?\n1)Per targa\n2)Indicizzata per data\n3)Indicizzata per cognome\n");
      scanf(" %d", &b);

      banneradmin();
      printf("Ecco la lista di tutti i %d veicoli salvati nel sistema:\n", countall(lista_auto));
      switch(b){
        case 1:
          printf("I veicoli contrassegnati da * sono stati inseriti dai dipendenti.\n");
          print_lista(lista_auto);
        break;

        case 2:
          printauto_perdata(indice_date);
        break;

        case 3:
          printauto_percognome(indice_cognomi);
        break;
      }

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();

          forth_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 4:
      banneradmin();

      third_menu(lista_auto, indice_date, indice_cognomi);
    break;

    case 5:
      printf("Arrivederci\n");

      uscita(lista_auto, indice_date, indice_cognomi);
    break;

    default:
      printf("Arrivederci.\n");

      uscita(lista_auto, indice_date, indice_cognomi);
    break;
  }
}

void third_menu(lista lista_auto, inddat indice_date, indsurn indice_cognomi){
  int a, b, keep, goon = 0;

  printf("Cosa vuoi fare?\n1)Inserire un nuovo veicolo\n2)Ritirare un veicolo\n3)Visualizzare lista veicoli presenti nel sistema\n4)Torna indietro\n5)Esci\n");
  scanf(" %d", &a);

  banneradmin();

  switch(a){
    case 1:
      banneradmin();

      forth_menu(lista_auto, indice_date, indice_cognomi);
    break;

    case 2:
      banneradmin();

      fifth_menu(lista_auto, indice_date, indice_cognomi);
    break;

    case 3:
      printf("Come si desidera ordinare?\n1)Per targa\n2)Indicizzata per data\n3)Indicizzata per cognome\n");
      scanf(" %d", &b);

      banneradmin();
      printf("Ecco la lista di tutti i %d veicoli salvati nel sistema:\n", countall(lista_auto));
      switch(b){
        case 1:
          printf("I veicoli contrassegnati da * sono stati inseriti dai dipendenti.\n");
          print_lista(lista_auto);
        break;

        case 2:
          printauto_perdata(indice_date);
        break;

        case 3:
          printauto_percognome(indice_cognomi);
        break;
      }


      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();

          third_menu(lista_auto, indice_date, indice_cognomi);
        break;

        case 2:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 4:
      banneradmin();
      freelista(lista_auto);
      freedate(indice_date);
      freecognomi(indice_cognomi);

      second_menu();
    break;

    case 5:
      printf("Arrivederci.\n");

      uscita(lista_auto, indice_date, indice_cognomi);
    break;

    default:
      printf("Arrivederci.\n");

      uscita(lista_auto, indice_date, indice_cognomi);
    break;
  }
}

void menu_pin(){
  int x, keep, goon = 0;

  PIN codice = initpin();

  banneradmin();

  printf("1)Visualizza PIN per i dipendenti attuale\n2)Cambia PIN\n3)Torna indietro\n");
  scanf(" %d", &x);
  switch(x){
    case 1:
      printf("Il PIN attuale e':\t%d\n", codice->PIN);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          free(codice);

          menu_pin();
        break;

        case 2:
          printf("Arrivederci.\n");
          free(codice);
          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          free(codice);
          exit(1);
        break;
      }
    break;

    case 2:
      resetPIN(&codice);
      storePIN(codice);

      sleep(4);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          free(codice);

          menu_pin();
        break;

        case 2:
          printf("Arrivederci.\n");
          free(codice);
          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          free(codice);
          exit(1);
        break;
      }
    break;

    case 3:
      banneradmin();
      free(codice);

      second_menu();
    break;
  }
}

void gestione_cassa(lista lista_auto){
  int l, keep, goon = 0;
  cassa manage;


  printf("Cosa vuoi fare?\n1)Visualizza crediti fruttati\n2)Visualizza cassa fisica\n3)Prelievo da cassa fisica\n4)Aggiorna cassa\n5)Torna indietro\n6)Esci\n");
  scanf(" %d", &l);

  banneradmin();

  manage = initcassa(lista_auto);

  switch(l){
    case 1:
      printf("I crediti accumulati fin'ora ammontano a %d euro.\n", manage->virtuale);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(manage);

          gestione_cassa(lista_auto);
        break;

        case 2:
          printf("Arrivederci.\n");
          freelista(lista_auto);
          freecassa(manage);

          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          freelista(lista_auto);
          freecassa(manage);

          exit(1);
        break;
      }
    break;

    case 2:
      printf("Questo e' l'ammontare di denaro fruttato: %d\n", manage->fisica);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(manage);

          gestione_cassa(lista_auto);
        break;

        case 2:
          printf("Arrivederci.\n");
          freelista(lista_auto);
          freecassa(manage);

          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          freelista(lista_auto);
          freecassa(manage);

          exit(1);
        break;
      }
    break;

    case 3:
      printf("Questo e' l'ammontare di denaro fruttato: %d\n\n", manage->fisica);
      prelievo(&manage);
      sleep(3);
      printf("Prelievo effettuato con successo!\n");

      sleep(3);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(manage);

          gestione_cassa(lista_auto);
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(manage);
          freelista(lista_auto);

          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          freecassa(manage);
          freelista(lista_auto);

          exit(1);
        break;
      }
    break;

    case 4:
      manage = initcassa(lista_auto);
      sleep(3);

      printf("Cassa aggiornata correttamente.\n");
      sleep(3);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(manage);

          gestione_cassa(lista_auto);
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(manage);
          freelista(lista_auto);

          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          freecassa(manage);
          freelista(lista_auto);

          exit(1);
        break;
      }
    break;

    case 5:
      banneradmin();
      freelista(lista_auto);
      freecassa(manage);

      second_menu();
    break;

    case 6:
      printf("Arrivederci.\n");
      freecassa(manage);
      freelista(lista_auto);

      exit(1);
    break;

    default:
      printf("Arrivederci.\n");
      freecassa(manage);
      freelista(lista_auto);

      exit(1);
    break;
  }
}

void second_menu(){
  int u, keep, goon = 0;
  sell car;
  cassa fondi;
  inddat indice_date = NULL;
  indsurn indice_cognomi = NULL;
  oste head;

  lista lista_auto = getlistafromfile(&indice_date, &indice_cognomi);

  printf("Cosa vuoi fare?\n1)Gestisci parco mezzi\n2)Gestisci cassa\n3)Visualizza auto in vendita\n4)Vendere auto\n5)Visualizza lista delle auto vendute\n6)Visualizza logs dei dipendenti nella data di oggi\n7)Visualizza elenco auto inserite dai dipendenti (ordinate per targa)\n8)Gestisci PIN dipendenti\n9)Aggiorna sistema\n10)Esci\n");
  scanf(" %d", &u);

  banneradmin();


  switch(u){
    case 1:
      banneradmin();

      third_menu(lista_auto, indice_date, indice_cognomi);
    break;

    case 2:
      banneradmin();
      freedate(indice_date);
      freecognomi(indice_cognomi);

      gestione_cassa(lista_auto);
    break;

    case 3:
      car = invendita();
      printf("Ecco l'elenco delle auto in vendita:\n");
      print_autoinvendita(car);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freevendite(car);
          freelista(lista_auto);
          freecognomi(indice_cognomi);
          freedate(indice_date);

          second_menu();
        break;

        case 2:
          printf("Arrivederci.\n");
          freevendite(car);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freevendite(car);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 4:
      car = invendita();
      fondi = initcassa(lista_auto);

      vendita_auto(&car, &fondi);
      sleep(4);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freevendite(car);
          freecassa(fondi);
          freecognomi(indice_cognomi);
          freedate(indice_date);

          second_menu();
        break;

        case 2:
          printf("Arrivederci.\n");
          freevendite(car);
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freevendite(car);
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 5:
      printf("Ecco la lista delle auto vendute:\n\n");
      fondi = initcassa(lista_auto);
      printstorico(fondi->chrono);
      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freecassa(fondi);
          freelista(lista_auto);
          freecognomi(indice_cognomi);
          freedate(indice_date);

          second_menu();
        break;

        case 2:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freecassa(fondi);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 6:
      head = read_guest();
      controllolog(head);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freelista(lista_auto);
          freeguest(head);
          freecognomi(indice_cognomi);
          freedate(indice_date);

          second_menu();
        break;

        case 2:
          printf("Arrivederci.\n");
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");
          freeguest(head);

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 7:
      printf("Ecco la lista delle auto inserite dai dipendenti:\n");
      print_lista_dip(lista_auto);

      sleep(2);
      printf("Premere INVIO per andare avanti...\n\n");
      getchar();
      while(goon != '\n')
        goon = getchar();

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          banneradmin();
          freelista(lista_auto);
          freecognomi(indice_cognomi);
          freedate(indice_date);

          second_menu();
        break;

        case 2:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;

        default:
          printf("Arrivederci.\n");

          uscita(lista_auto, indice_date, indice_cognomi);
        break;
      }
    break;

    case 8:
      banneradmin();
      freelista(lista_auto);
      freecognomi(indice_cognomi);
      freedate(indice_date);

      menu_pin();
    break;

    case 9:
      lista_auto = getlistafromfile(&indice_date, &indice_cognomi);
      banneradmin();
      freelista(lista_auto);
      freecognomi(indice_cognomi);
      freedate(indice_date);

      second_menu();

    break;

    case 10:
      printf("Arrivederci\n");

      uscita(lista_auto, indice_date, indice_cognomi);
    break;

    default:
      printf("Arrivederci.\n");

      uscita(lista_auto, indice_date, indice_cognomi);
    break;
  }
}

void first_menu(char save[15]){
  int y, keep;

  banneradmin();

  printf("1)Accedere al sistema\n2)Reimpostare password admin\n3)Esci\n");
  scanf(" %d", &y);
  switch(y){
    case 1:
      banneradmin();

      second_menu();
    break;

    case 2:
      reset_pass(save);

      sleep(5);

      banneradmin();

      printf("1.Torna indietro\n2.Esci\n");
      scanf(" %d", &keep);
      switch(keep){
        case 1:
          system("clear");
          first_menu(save);
        break;

        case 2:
          printf("Arrivederci.\n");
          exit(1);
        break;

        default:
          printf("Arrivederci.\n");
          exit(1);
        break;
      }
    break;

    case 3:
      printf("Arrivederci.\n");
      exit(1);
    break;

    default:
      printf("Arrivederci.\n");
      exit(1);
    break;
  }
}
