#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


#pragma comment (lib, "Ws2_32.lib")


#define NACHRICHTENLAENGE 4096

int main()
{
	// winsock initialisieren
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	SOCKADDR_IN server;
	SOCKET listenSocket, neuesSocket;
	char nachricht[NACHRICHTENLAENGE];
	int iRC;
	int iBytesIn;

	BOOL bSchleifeAktiv = TRUE;

	fd_set aktiveSockets, copieSockets;

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		printf("Can't Initialize winsock! Quitting\n");
		return(1);
	};


	// Listen Socke generieren
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Can't create a socket! Quitting\n");
		return(1);
	};

	server.sin_family = AF_INET;
	server.sin_port = htons(50000);
	server.sin_addr.S_un.S_addr = INADDR_ANY;

	iRC = bind(listenSocket, (SOCKADDR *)&server, sizeof(server));
	if (iRC == -1)
        return 1;

	// winsock zu listening zuweisen
	iRC = listen(listenSocket, SOMAXCONN);
	if (iRC == -1)
        return 1;

	// master file descriptor... set und zero
	// hier werden alle aktiven Socken gespeichert fur die TCP/IP Funktionen
	FD_ZERO(&(aktiveSockets));

	// Das erste socket muss ein listing socket sein
	FD_SET(listenSocket, &(aktiveSockets));

	printf("Server steht bereit\n");

	// Schleife wird so lange durchlaufen wie bSchleifeAktiv = true ist
	while (bSchleifeAktiv)
	{
		// EXTREM WICHTIG; master file descriptor muss immer kopiert werden,
		// damit alle aktiven Sockets in einer Liste gehalten werden. Die Variable
		// copy wird im Select Aufruf neu belegt mit den Sockets, auf denen eine
		// Information angekommen ist
		copieSockets = aktiveSockets;

		// Pr�fung, wer spricht zu uns. Anwort kommt aus dem Select
		// Select wartet bis eine Nachricht oder ein Verbindungsaufbau kommt
		select(0, &copieSockets, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);

		// Loop f�r alle eingehenden Verbindungen
		for (int i = 0; i < copieSockets.fd_count; i++)
		{
			// Verbindungsanfrage
			if (copieSockets.fd_array[i] == listenSocket)
			{
					// Neue Verbindung akzeptieren
					neuesSocket = accept(listenSocket, (SOCKADDR *)NULL, (int *)NULL);
					// Neue Verbindung zur aktiven Socket List hinzuf�gen
					FD_SET(neuesSocket, &aktiveSockets);
			}
			else // Nachricht da, oder Verbindung wurde geschlossen
			{
			    // Nachrichtenpuffer nullen
				memset(nachricht,0 , NACHRICHTENLAENGE);

				// Nachricht oder Verbindungsabbruch empfangen
				iBytesIn = recv(copieSockets.fd_array[i], nachricht, NACHRICHTENLAENGE, 0);
				if (iBytesIn <= 0) //Verbindungsabbruch
				{
					FD_CLR(aktiveSockets.fd_array[i], &aktiveSockets);
					closesocket(copieSockets.fd_array[i]);
					if (!aktiveSockets.fd_count)
                    {
                        printf("keine Client Sockets mehr aktiv\n");
                        printf("!!!! Server sagt tschuess\n");
                        return 1;
                    }
				}
				else // Nachricht angekommen
				{
					printf("Client Socket <%ud> Nachricht <%s>\n",
                            (unsigned int)copieSockets.fd_array[i], nachricht);
					// zuruecksenden der Nachricht

					if (!strncmp(nachricht, "Ende", 4))
                    {
                        send(copieSockets.fd_array[i], nachricht, sizeof(nachricht), 0);
                        // nun sollte die Schleife beendet werden
                        bSchleifeAktiv =  FALSE;
                    }
                    else
                        send(copieSockets.fd_array[i], nachricht, iBytesIn, 0);
				} // else von if (bytesIn <= 0)
			} // else von if (copy.fd_array[i] == listenSocke)
		} // for (int i = 0; i < copySocks.fd_count; i++)
	} //end while(running)

	for (int i =0; i<aktiveSockets.fd_count; i++)
	{
		closesocket(aktiveSockets.fd_array[i]);
	}

	// Cleanup winsock
	WSACleanup();

	printf("\n !!!! Server closed \n");
	system("pause");
}

