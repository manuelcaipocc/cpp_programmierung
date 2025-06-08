#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
// #include <ws2tcpip.h>
// #include <winsock2.h>

#define NACHRICHTENLAENGE 4096

int main()
{
    WSADATA     wsData;
	WORD ver = MAKEWORD(2, 2);
	SOCKET              clientSocket;
	struct sockaddr_in  serverAdresse;
	char                nachricht[NACHRICHTENLAENGE];
	int iRC;


	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		printf("Can't Initialize winsock! Quitting\n");
		return(1);
	};

	// Erzeugt ein Socket fur die Verbindung
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Fehler: Das Socket-Anlegen f�r die Kommunikation hat nicht funktioniert\n");
		return 1;
	}

		//Socket mit den richtigen Parametern belegen
	memset((char *)&serverAdresse, 0, sizeof(serverAdresse));
	serverAdresse.sin_family = AF_INET;
//	sock.sin_addr.s_addr = htonl(LOCAL_COMPUTER);
    // Loop  back auf dem eigenen Computer
    // damit kann man zwischen Prozessen auf dem eigenen Computer
    // mit tcp/ip kommunizieren
	serverAdresse.sin_addr.s_addr = inet_addr("127.0.0.1");
//	inet_pton(AF_INET, localAdress, &sock.sin_addr);
	serverAdresse.sin_port = htons(50000); // Server port

	// Anfrage zum Verbindungsaufbau
	if (connect(clientSocket, (struct sockaddr *) &serverAdresse, sizeof(serverAdresse)) < 0)
	{
		printf("Verbindungsaufbau Connect zum TCP Server hat nicht funktioniert\n");
		return 1;
	}
	printf("Verbindungsaufbau Connect mit TCP Server erfolgreich\n");


	while (TRUE)
    {
        // so nun wollen wir wissen, was wir senden wollen
        printf("\nGeben Sie eine Nachricht ein: ");
        fgets(nachricht, NACHRICHTENLAENGE, stdin);

        if (!strncmp(nachricht, "kill",4))
            break;

        // so und nun senden wir es an den TCP Server
        // strelen()-1 weil das newline nicht mituebertragen werden soll
        iRC = send(clientSocket, nachricht, strlen(nachricht)-1,0);
        if (iRC == -1)
        {
            printf("Fehler beim Senden der Nachricht\n");
        }
        memset(nachricht, 0, sizeof(nachricht));
        iRC = recv(clientSocket, nachricht, sizeof(nachricht),0 );
        if (iRC == -1)
        {
            printf("Verbindung geschlossen\n");
            return 1;
        }
        else
        {
            printf("Server Antwort-Nachricht <%s>\n", nachricht);
        }

    }

    closesocket(clientSocket);
    printf("!!!!! Client sagt tschuess\n");

	// Cleanup winsock
	WSACleanup();

	system("pause");
}
