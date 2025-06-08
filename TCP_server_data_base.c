#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#define NACHRICHTENLAENGE 4096

// ----------- BEFEHL-DEFINITIONEN -----------
#define CMD_ADD_MACHINE "ADD_MACHINE"
#define CMD_DELETE_MACHINE "DELETE_MACHINE"
#define CMD_DELETE_ALL_MACHINES "DELETE_ALL_MACHINES"
#define CMD_SAVE_MACHINES "SAVE_MACHINES"
#define CMD_LOAD_MACHINES "LOAD_MACHINES"
#define CMD_LIST_MACHINES "LIST_MACHINES"

#define CMD_ADD_FAILURE "ADD_FAILURE"
#define CMD_DELETE_FAILURE "DELETE_FAILURE"
#define CMD_DELETE_ALL_FAILURES "DELETE_ALL_FAILURES"
#define CMD_SAVE_FAILURES "SAVE_FAILURES"
#define CMD_LOAD_FAILURES "LOAD_FAILURES"
#define CMD_LIST_FAILURES "LIST_FAILURES"

#define CMD_NEU "NEU"
#define CMD_DELETE_ALL "DELETE_ALL"
#define CMD_DELETE_ONE "DELETE"
#define CMD_SAVE "SAVE"
#define CMD_LOAD "LOAD"
#define CMD_LISTE "LISTE"
#define CMD_ENDE "ENDE"

//En tu programa C con sockets, un telegrama es una cadena 
//con información estructurada que representa una orden, 
//consulta o evento, que el cliente o servidor envían y procesan.
// ----------------- STRUCTS -----------------

typedef struct Maschine {
    int code;
    char name[100];
    struct Maschine* next;
    struct Maschine* prev;
} Maschine;

typedef struct Fehler {
    int code;
    char beschreibung[200];
    struct Fehler* next;
    struct Fehler* prev;
} Fehler;

typedef struct Stillstand {
    int maschineCode;
    int fehlerCode;
    char startZeit[20];  // antes float
    char endZeit[20];    // antes float
    Maschine* maschine;
    Fehler* fehler;
    struct Stillstand* next;
    struct Stillstand* prev;
} Stillstand;

// ----------------- GLOBAL LISTENKÖPFE -----------------
Maschine* kopfMaschine = NULL;
Fehler* kopfFehler = NULL;
Stillstand* kopfStillstand = NULL;

// ----------------- HILFSFUNKTIONEN -----------------
// it gives us the pointer not a complet copy
Maschine* sucheMaschine(int code) {
    Maschine* tmp = kopfMaschine;
    while (tmp) {
        if (tmp->code == code)
            {return tmp;}
        // its the same than ((*tmp).code == code) return tmp;
        else {tmp=tmp->next;}
    }
    return NULL;
}

Fehler* sucheFehler(int code) {
    Fehler* tmp = kopfFehler;
    while (tmp) {
        if (tmp->code == code) 
            {return tmp;}
        else {tmp=tmp->next;}
    }
    return NULL;
}

// ----------------- MASCHINEN-FUNKTIONEN -----------------
void insertMaschine(int code, const char* name) {
    Maschine* neu = malloc(sizeof(Maschine));
    // struct Maschine *m = malloc(sizeof(struct Maschine));
    //Reserva memoria para una estructura Maschine y guarda la dirección en el puntero m
    neu->code = code;
    strncpy(neu->name, name, 99); // copia hasta 99 caracteres
    neu->name[99] = '\0'; // nos aseguranos que termine en \0 para saber donde termina la cadena
    neu->next = kopfMaschine;
    neu->prev = NULL;
    if (kopfMaschine != NULL) kopfMaschine->prev = neu;
    kopfMaschine = neu;
}

void deleteMaschine(int code) {
    Stillstand* s = kopfStillstand;
    while (s) {
        if (s->maschineCode == code) return; // FK constraint
        s = s->next;
    }
    Maschine* tmp = kopfMaschine;
    while (tmp != NULL) {
        if (tmp->code == code) {
            if (tmp->prev) tmp->prev->next = tmp->next;
            else kopfMaschine = tmp->next;
            if (tmp->next) tmp->next->prev = tmp->prev;
            free(tmp);
            return;
        }
        tmp = tmp->next;
    }
}

void deleteAllMaschinen() {
    Maschine* tmp = kopfMaschine;
    while (tmp != NULL) {
        Maschine* zuLoeschen = tmp;
        tmp = tmp->next;
        free(zuLoeschen);
    }
    kopfMaschine = NULL;
}

void saveMaschinenToFile() {
    FILE* f = fopen("001 maschinen.txt", "w");
    fprintf(f, "# Maschinencode;Name\n"); 
    Maschine* tmp = kopfMaschine;
    while (tmp != NULL) {
        fprintf(f, "%d;%s\n", tmp->code, tmp->name);
        tmp = tmp->next;
    }
    fclose(f);
}

void loadMaschinenFromFile() {
    FILE* f = fopen("001 maschinen.txt", "r");

    if (!f) return;
    deleteAllMaschinen();

    char zeile[256];
    fgets(zeile, sizeof(zeile), f); 

    int code; char name[100];
    while (fscanf(f, "%d;%99[^\n]\n", &code, name) == 2) {
        insertMaschine(code, name);
    }
    fclose(f);
}

void zeigeMaschinenAlsText(char* puffer, int maxLen) {
    Maschine* tmp = kopfMaschine;
    strcpy(puffer, "--- Maschinen ---\n\n");
    char zeile[150];
    while (tmp) {
        snprintf(zeile, sizeof(zeile), "Code: %d | Name: %s\n", tmp->code, tmp->name);
        if (strlen(puffer) + strlen(zeile) < maxLen - 1) {
            strcat(puffer, zeile);
        }
        tmp = tmp->next;
    }

    if (kopfMaschine == NULL) {
        strcat(puffer, "(Keine Maschinen vorhanden)\n");
    }
}

// ----------------- FEHLER-FUNKTIONEN -----------------
void insertFehler(int code, const char* beschreibung) {
    Fehler* neu = malloc(sizeof(Fehler));
    neu->code = code;
    strncpy(neu->beschreibung, beschreibung, 199);
    neu->beschreibung[199] = '\0';
    neu->next = kopfFehler;
    neu->prev = NULL;
    if (kopfFehler != NULL) kopfFehler->prev = neu;
    kopfFehler = neu;
}

void deleteFehler(int code) {
    Stillstand* s = kopfStillstand;
    while (s) {
        if (s->fehlerCode == code) return; // FK constraint
        s = s->next;
    }
    Fehler* tmp = kopfFehler;
    while (tmp != NULL) {
        if (tmp->code == code) {
            if (tmp->prev) tmp->prev->next = tmp->next;
            else kopfFehler = tmp->next;
            if (tmp->next) tmp->next->prev = tmp->prev;
            free(tmp);
            return;
        }
        tmp = tmp->next;
    }
}

void deleteAllFehler() {
    Fehler* tmp = kopfFehler;
    while (tmp != NULL) {
        Fehler* zuLoeschen = tmp;
        tmp = tmp->next;
        free(zuLoeschen);
    }
    kopfFehler = NULL;
}

void saveFehlerToFile() {
    FILE* f = fopen("002 fehler.txt", "w");
    fprintf(f, "# Fehlercode;Beschreibung\n"); 
    Fehler* tmp = kopfFehler;
    while (tmp != NULL) {
        fprintf(f, "%d;%s\n", tmp->code, tmp->beschreibung);
        tmp = tmp->next;
    }
    fclose(f);
}

void loadFehlerFromFile() {
    FILE* f = fopen("002 fehler.txt", "r");
    if (!f) return;
    deleteAllFehler();

    char zeile[256];
    fgets(zeile, sizeof(zeile), f); 

    int code; char beschreibung[200];
    while (fscanf(f, "%d;%199[^\n]\n", &code, beschreibung) == 2) {
        insertFehler(code, beschreibung);
    }
    fclose(f);
}

void zeigeFehlerAlsText(char* puffer, int maxLen) {
    Fehler* tmp = kopfFehler;
    strcpy(puffer, "--- Fehler ---\n\n");
    char zeile[200];
    while (tmp) {
        snprintf(zeile, sizeof(zeile), "Code: %d | Beschreibung: %s\n", tmp->code, tmp->beschreibung);
        if (strlen(puffer) + strlen(zeile) < maxLen - 1) {
            strcat(puffer, zeile);
        }
        tmp = tmp->next;
    }

    if (kopfFehler == NULL) {
        strcat(puffer, "(Keine Fehler vorhanden)\n");
    }
}


// ----------------- STILLSTAND-FUNKTIONEN -----------------
void insertStillstand(int maschineCode, int fehlerCode, const char* start, const char* end) {
    Maschine* m = sucheMaschine(maschineCode);
    Fehler* f = sucheFehler(fehlerCode);
    if (!m || !f) return; // FK validation

    Stillstand* neu = malloc(sizeof(Stillstand));
    neu->maschineCode = maschineCode;
    neu->fehlerCode = fehlerCode;
    strncpy(neu->startZeit, start, 19);
    neu->startZeit[19] = '\0';
    strncpy(neu->endZeit, end, 19);
    neu->endZeit[19] = '\0';
    neu->maschine = m;
    neu->fehler = f;
    neu->next = kopfStillstand;
    neu->prev = NULL;
    if (kopfStillstand != NULL) kopfStillstand->prev = neu;
    kopfStillstand = neu;
}

void deleteAllStillstand() {
    Stillstand* tmp = kopfStillstand;
    while (tmp != NULL) {
        Stillstand* zuLoeschen = tmp;
        tmp = tmp->next;
        free(zuLoeschen);
    }
    kopfStillstand = NULL;
}

void deleteOneStillstand(int maschineCode) {
    Stillstand* tmp = kopfStillstand;
    while (tmp != NULL) {
        if (tmp->maschineCode == maschineCode) {
            //las flechitas son como simbolos de logica pero can de izquierda a derecha
            //tmp->prev esto da un primer resultado y de este quiero el next
            if (tmp->prev) tmp->prev->next = tmp->next;
            else kopfStillstand = tmp->next;
            if (tmp->next) tmp->next->prev = tmp->prev;
            free(tmp);
            return;
        }
        tmp = tmp->next;
    }
}

void saveStillstandToFile() {
    FILE* f = fopen("003 stillstand.txt", "w");
    fprintf(f, "# MaschinenCode;FehlerCode;Startzeit;Endzeit\n");
    Stillstand* tmp = kopfStillstand;
    while (tmp != NULL) {
        fprintf(f, "%d;%d;%s;%s\n", tmp->maschineCode, tmp->fehlerCode, tmp->startZeit, tmp->endZeit);
        tmp = tmp->next;
    }
    fclose(f);
}


void loadStillstandFromFile() {
    FILE* f = fopen("003 stillstand.txt", "r");
    if (!f) return;
    deleteAllStillstand();

    char zeile[256];
    fgets(zeile, sizeof(zeile), f); // ignorar cabecera

    int mCode, fCode;
    char start[20], end[20];
    while (fscanf(f, "%d;%d;%19[^;];%19[^\n]\n", &mCode, &fCode, start, end) == 4) {
        insertStillstand(mCode, fCode, start, end);
    }
    fclose(f);
}



void zeigeStillstaendeAlsText(char* puffer, int maxLen) {
    Stillstand* tmp = kopfStillstand;
    strcpy(puffer, "--- Stillstände ---\n\n");
    char zeile[300];
    while (tmp) {
        snprintf(zeile, sizeof(zeile),
                 "Maschine: %d (%s) | Fehler: %d (%s) | Start: %s | Ende: %s\n",
                 tmp->maschineCode,
                 tmp->maschine ? tmp->maschine->name : "?",
                 tmp->fehlerCode,
                 tmp->fehler ? tmp->fehler->beschreibung : "?",
                 tmp->startZeit, tmp->endZeit);
        if (strlen(puffer) + strlen(zeile) < maxLen - 1) {
            strcat(puffer, zeile);
        }
        tmp = tmp->next;
    }

    if (kopfStillstand == NULL) {
        strcat(puffer, "(Keine Stillstände vorhanden)\n");
    }
}


// ----------------- SERVER MAIN -----------------
int main() {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    SOCKET listenSocket, neuesSocket;
    SOCKADDR_IN server;
    char nachricht[NACHRICHTENLAENGE];
    int iBytesIn;
    BOOL bSchleifeAktiv = TRUE;
    fd_set aktiveSockets, copieSockets;

  
    if (WSAStartup(ver, &wsData) != 0) {
        printf("Can't Initialize winsock! Quitting\n");
        return 1;
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        printf("Can't create a socket! Quitting\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(50000);
    server.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(listenSocket, (SOCKADDR*)&server, sizeof(server)) == -1)
        return 1;

    if (listen(listenSocket, SOMAXCONN) == -1)
        return 1;

    FD_ZERO(&aktiveSockets);
    FD_SET(listenSocket, &aktiveSockets);

    printf("Server gestartet - Server steht bereit ...\n");

    while (bSchleifeAktiv) {
        copieSockets = aktiveSockets;
        //no avisar por errores especificos
        select(0, &copieSockets, NULL, NULL, NULL);

        for (int i = 0; i < copieSockets.fd_count; i++) {
            if (copieSockets.fd_array[i] == listenSocket) {
                neuesSocket = accept(listenSocket, NULL, NULL);
                FD_SET(neuesSocket, &aktiveSockets);
            } else {
                memset(nachricht, 0, NACHRICHTENLAENGE);
                iBytesIn = recv(copieSockets.fd_array[i], nachricht, NACHRICHTENLAENGE, 0);
                // ingreso de mi socket aqui esta mi mensaje "iBytesIn"
                if (iBytesIn <= 0) {
                    closesocket(copieSockets.fd_array[i]);
                    FD_CLR(copieSockets.fd_array[i], &aktiveSockets);
                    continue;
                }

                // BEFEHLSVERARBEITUNG
                // con strncmp puedo comparar los primeros n caracteres que defino con strlen despues
                if (strncmp(nachricht, CMD_ADD_MACHINE, strlen(CMD_ADD_MACHINE)) == 0) {
                    int code; char name[100];
                    sscanf(nachricht, "ADD_MACHINE;%d;%99[^\\n]", &code, name);
                    insertMaschine(code, name);
                    strcpy(nachricht, "Maschine hinzugefuegt.");
                } else if (strncmp(nachricht, CMD_DELETE_MACHINE, strlen(CMD_DELETE_MACHINE)) == 0) {
                    int code;
                    sscanf(nachricht, "DELETE_MACHINE;%d", &code);
                    deleteMaschine(code);
                    strcpy(nachricht, "Maschine geloescht.");
                } else if (strncmp(nachricht, CMD_DELETE_ALL_MACHINES, strlen(CMD_DELETE_ALL_MACHINES)) == 0) {
                    deleteAllMaschinen();
                    strcpy(nachricht, "Alle Maschinen gehen weg, asu casa.");
                } else if (strncmp(nachricht, CMD_SAVE_MACHINES, strlen(CMD_SAVE_MACHINES)) == 0) {
                    saveMaschinenToFile();
                    strcpy(nachricht, "Maschinen gespeichert.");
                } else if (strncmp(nachricht, CMD_LOAD_MACHINES, strlen(CMD_LOAD_MACHINES)) == 0) {
                    loadMaschinenFromFile();
                    strcpy(nachricht, "Maschinen geladen.");
                } else if (strncmp(nachricht, CMD_ADD_FAILURE, strlen(CMD_ADD_FAILURE)) == 0) {
                    int code; char beschreibung[200];
                    sscanf(nachricht, "ADD_FAILURE;%d;%199[^\\n]", &code, beschreibung);
                    insertFehler(code, beschreibung);
                    strcpy(nachricht, "Fehler hinzugefuegt.");
                } else if (strncmp(nachricht, CMD_DELETE_FAILURE, strlen(CMD_DELETE_FAILURE)) == 0) {
                    int code;
                    sscanf(nachricht, "DELETE_FAILURE;%d", &code);
                    deleteFehler(code);
                    strcpy(nachricht, "Fehler geloescht.");
                } else if (strncmp(nachricht, CMD_DELETE_ALL_FAILURES,  strlen(CMD_DELETE_ALL_FAILURES)) == 0) {
                    deleteAllFehler();
                    strcpy(nachricht, "Alle Fehler geloescht.");
                } else if (strncmp(nachricht, CMD_SAVE_FAILURES, strlen(CMD_SAVE_FAILURES)) == 0) {
                    saveFehlerToFile();
                    strcpy(nachricht, "Fehler gespeichert.");
                } else if (strncmp(nachricht, CMD_LOAD_FAILURES, strlen(CMD_LOAD_FAILURES)) == 0) {
                    loadFehlerFromFile();
                    strcpy(nachricht, "Fehler geladen.");

                } else if (strncmp(nachricht, CMD_NEU, strlen(CMD_NEU)) == 0) {
                    int masch, fehler;
                    char start[20], end[20];
                
                    // Importante: usar %19[^;] para leer hasta punto y coma sin pasarse del tamaño del string
                    if (sscanf(nachricht, "NEU;%d;%d;%19[^;];%19[^\n]", &masch, &fehler, start, end) == 4) {
                        insertStillstand(masch, fehler, start, end);
                        strcpy(nachricht, "Stillstand gespeichert.");
                    } else {
                        strcpy(nachricht, "FEHLER: Ungültiges Format für Stillstand.");
                    }             
                } else if (strncmp(nachricht, CMD_DELETE_ALL, strlen(CMD_DELETE_ALL)) == 0) {
                    deleteAllStillstand();
                    strcpy(nachricht, "Alle Stillstaende geloescht.");
                } else if (strncmp(nachricht, CMD_DELETE_ONE, strlen(CMD_DELETE_ONE)) == 0) {
                    int masch;
                    sscanf(nachricht, "DELETE;%d", &masch);
                    deleteOneStillstand(masch);
                    strcpy(nachricht, "Eintrag geloescht.");
                } else if (strncmp(nachricht, CMD_SAVE, strlen(CMD_SAVE)) == 0) {
                    saveStillstandToFile();
                    strcpy(nachricht, "Gespeichert.");
                } else if (strncmp(nachricht, CMD_LOAD, strlen(CMD_LOAD)) == 0) {
                    loadStillstandFromFile();
                    strcpy(nachricht, "Geladen.");
                } else if (strncmp(nachricht, CMD_ENDE, strlen(CMD_ENDE)) == 0) {
                    strcpy(nachricht, "Server beendet.");
                    bSchleifeAktiv = FALSE;
                } 
                
                 else if (strncmp(nachricht, CMD_LIST_MACHINES, strlen(CMD_LIST_MACHINES)) == 0) {
                zeigeMaschinenAlsText(nachricht, NACHRICHTENLAENGE);
            
                } else if (strncmp(nachricht, CMD_LIST_FAILURES, strlen(CMD_LIST_FAILURES)) == 0) {
                zeigeFehlerAlsText(nachricht, NACHRICHTENLAENGE);
            
                } else if (strncmp(nachricht, CMD_LISTE, strlen(CMD_LISTE)) == 0) {
                zeigeStillstaendeAlsText(nachricht, NACHRICHTENLAENGE);
                }
            
                else {
                    strcpy(nachricht, "Unbekannter Befehl.");
                }
                

                send(copieSockets.fd_array[i], nachricht, strlen(nachricht), 0);
            }
        }
    }

    for (int i = 0; i < aktiveSockets.fd_count; i++) {
        closesocket(aktiveSockets.fd_array[i]);
    }
    WSACleanup();
    return 0;
}
