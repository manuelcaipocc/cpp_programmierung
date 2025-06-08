#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define NACHRICHTENLAENGE 4096

void printMenu() {
    printf("\n========= Please Load the files first to try all funtionalities=========\n");
    printf("---------------------------------------------------------------------------------");
    printf("\n========= Manuel Caipo Failures Data Base - APE Matrikelnummer: 286577=========\n");

    printf("\n[1] Maschinen-Katalog    [2] Fehler-Katalog      [3] Stillstand-Verwaltung\n");
    printf(" 1.1 Laden               2.1 Laden               3.1 Laden\n");
    printf(" 1.2 Eingeben            2.2 Eingeben            3.2 Eingeben\n");
    printf(" 1.3 Loeschen            2.3 Loeschen            3.3 Loeschen\n");
    printf(" 1.4 Speichern           2.4 Speichern           3.4 Speichern\n");
    printf(" 1.5 Alle loeschen       2.5 Alle loeschen       3.5 Alle loeschen\n");
    printf(" 1.6 Anzeigen            2.6 Anzeigen            3.6 Anzeigen\n");
    
    printf("\n[4] Programm beenden\n");
    printf("Bitte geben Sie Ihre Auswahl ein (z.B. 1.2): ");
}

int main() {
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    SOCKET clientSocket;
    struct sockaddr_in serverAdresse;
    char nachricht[NACHRICHTENLAENGE];
    int iRC;

    if (WSAStartup(ver, &wsData) != 0) {
        printf("Can't Initialize winsock! Quitting\n");
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Fehler: Das Socket-Anlegen f\xe4r die Kommunikation hat nicht funktioniert\n");
        return 1;
    }

    memset(&serverAdresse, 0, sizeof(serverAdresse));
    serverAdresse.sin_family = AF_INET;
    serverAdresse.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAdresse.sin_port = htons(50000);

    if (connect(clientSocket, (struct sockaddr*)&serverAdresse, sizeof(serverAdresse)) < 0) {
        printf("Verbindungsaufbau Connect zum TCP Server hat nicht funktioniert\n");
        return 1;
    }
    printf("Verbindungsaufbau Connect mit TCP Server erfolgreich\n");

    char auswahl[10];
    while (1) {
        printMenu();
        fgets(auswahl, sizeof(auswahl), stdin);

// strncmp(auswahl, "1.1", 3):
// Compara los primeros 3 caracteres de la cadena 'auswahl' con "1.1".
// Devuelve 0 si las cadenas son iguales en esos primeros 3 caracteres.

// strcpy(nachricht, "LOAD_MACHINES"):
// Copia la cadena "LOAD_MACHINES" dentro del array 'nachricht'.
// Asegura que 'nachricht' contenga exactamente esa cadena, terminada en '\0'.

        if (strncmp(auswahl, "1.1", 3) == 0) {
            strcpy(nachricht, "LOAD_MACHINES");
        } else if (strncmp(auswahl, "1.2", 3) == 0) {
            int code;
            char name[100];
            printf("Maschinencode: "); scanf("%d", &code); getchar();
            printf("Name: "); fgets(name, sizeof(name), stdin);
            /* strcspn encuentra la posición del primer '\n' en 'name' y
            lo reemplaza con '\0' para eliminar el salto de línea */
            name[strcspn(name, "\n")] = 0;
            snprintf(nachricht, sizeof(nachricht), "ADD_MACHINE;%d;%s", code, name);
            // snprintf(destino, tamaño, "texto con %d y %s", numero, texto);
            //Construye una cadena de texto con formato y la guarda en destino,
            // respetando un límite de caracteres para evitar errores de memoria.
            // beispiel nachricht = "ADD_MACHINE;123;Motor"

        } else if (strncmp(auswahl, "1.3", 3) == 0) {
            int code;
            printf("Maschinencode zum Loeschen: "); scanf("%d", &code); getchar();
            snprintf(nachricht, sizeof(nachricht), "DELETE_MACHINE;%d", code);
        } else if (strncmp(auswahl, "1.4", 3) == 0) {
            strcpy(nachricht, "SAVE_MACHINES");
        } else if (strncmp(auswahl, "1.5", 3) == 0) {
            strcpy(nachricht, "DELETE_ALL_MACHINES");

        } else if (strncmp(auswahl, "2.1", 3) == 0) {
            strcpy(nachricht, "LOAD_FAILURES");
        } else if (strncmp(auswahl, "2.2", 3) == 0) {
            int code;
            char beschreibung[200];
            printf("Fehlercode: "); scanf("%d", &code); getchar();
            printf("Beschreibung: "); fgets(beschreibung, sizeof(beschreibung), stdin);
            beschreibung[strcspn(beschreibung, "\n")] = 0;
            snprintf(nachricht, sizeof(nachricht), "ADD_FAILURE;%d;%s", code, beschreibung);
        } else if (strncmp(auswahl, "2.3", 3) == 0) {
            int code;
            printf("Fehlercode zum Loeschen: "); scanf("%d", &code); getchar();
            snprintf(nachricht, sizeof(nachricht), "DELETE_FAILURE;%d", code);
        } else if (strncmp(auswahl, "2.4", 3) == 0) {
            strcpy(nachricht, "SAVE_FAILURES");
        } else if (strncmp(auswahl, "2.5", 3) == 0) {
            strcpy(nachricht, "DELETE_ALL_FAILURES");

        } else if (strncmp(auswahl, "3.1", 3) == 0) {
            strcpy(nachricht, "LOAD");
        
        } else if (strncmp(auswahl, "3.2", 3) == 0) {
            int maschine, fehler;
            float start, end;
            printf("MaschinenCode: "); scanf("%d", &maschine);
            printf("FehlerCode: "); scanf("%d", &fehler);
            printf("Startzeit (z.B. 10.5): "); scanf("%f", &start);
            printf("Endzeit (z.B. 12.3): "); scanf("%f", &end);
            getchar();
            snprintf(nachricht, sizeof(nachricht), "NEU;%d;%d;%.2f;%.2f", maschine, fehler, start, end);
        } else if (strncmp(auswahl, "3.3", 3) == 0) {
            int masch;
            printf("MaschinenCode zum Loeschen: "); scanf("%d", &masch); getchar();
            snprintf(nachricht, sizeof(nachricht), "DELETE;%d", masch);
        } else if (strncmp(auswahl, "3.4", 3) == 0) {
            strcpy(nachricht, "SAVE");
        } else if (strncmp(auswahl, "3.5", 3) == 0) {
            strcpy(nachricht, "DELETE_ALL");
        } else if (strncmp(auswahl, "1.6", 3) == 0) {
                strcpy(nachricht, "LIST_MACHINES");
        } else if (strncmp(auswahl, "2.6", 3) == 0) {
                strcpy(nachricht, "LIST_FAILURES");
        } else if (strncmp(auswahl, "3.6", 3) == 0) {
                strcpy(nachricht, "LISTE");

        } else if (strncmp(auswahl, "4", 1) == 0) {
            strcpy(nachricht, "ENDE");
            send(clientSocket, nachricht, strlen(nachricht), 0);
            break;
        } else {
            printf("Ungueltige Auswahl!\n");
            continue;
        }
        
        //strlen calcula la longitud del mensaje sin el termino nulo
        // 0 al final porque es el flag de control, sin operaciones especiales, solo lo envia
        iRC = send(clientSocket, nachricht, strlen(nachricht), 0);
        if (iRC == -1) {
            printf("Fehler beim Senden der Nachricht\n");
        }

        memset(nachricht, 0, sizeof(nachricht));
        iRC = recv(clientSocket, nachricht, sizeof(nachricht), 0);
        if (iRC == -1) {
            printf("Verbindung geschlossen\n");
            break;
        } else {
            printf("Antwort vom Server: %s\n", nachricht);
            printf("Press enter to continue with other requirement...");
            getchar();  // <<< this line create the pause to the enter of the user for a new command
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    printf("!!!!! Client sagt tsch\xfcss\n");
    return 0;
}