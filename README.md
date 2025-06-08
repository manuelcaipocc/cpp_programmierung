# TCP/IP Client-Server Database Application

## Overview

This project implements a client-server architecture using C/C++ and the Windows Sockets API (WinSock). It features a file-backed linked list database to manage three core data domains:

- **Machine Catalog** (`Maschinen`)
- **Failure Catalog** (`Fehler`)
- **Downtime Records** (`Stillstände`)

Each data domain supports loading from file, creation, deletion, listing, and saving to file.

This project was developed as part of the course **C/C++ Programmierung** at [**Hochschule Furtwangen University**](https://www.hs-furtwangen.de/), supervised by [Prof. Dr. Jörg Friedrich](https://www.hs-furtwangen.de/personen/profil/1250-joergfriedrich).

---

## Author

**Manuel Alberto Caipo Ccoa**  
Master of Advanced Precision Engineering  
Matrikelnummer: 286577  
Email: manuelcaipocc@outlook.com

---

## Project Files

- `TCP_client_data_base_caipo.c` - Client-side application with interactive command-line interface
- `TCP_server_data_base_caipo.c` - Server-side application handling concurrent client requests using `select()`

---

## Functionalities

Each data domain supports these operations:

| Operation   | Maschinen | Fehler | Stillstände |
|-------------|-----------|--------|-------------|
| Load        | 1.1       | 2.1    | 3.1         |
| Add         | 1.2       | 2.2    | 3.2         |
| Delete      | 1.3       | 2.3    | 3.3         |
| Save        | 1.4       | 2.4    | 3.4         |
| Delete All  | 1.5       | 2.5    | 3.5         |
| List        | 1.6       | 2.6    | 3.6         |

---

## Data Domains

### Machine Catalog (001 maschinen.txt)
```text
# Maschinencode;Name
101;Fräsmaschine
102;Bohrmaschine
103;CNC-Router
104;Schweißroboter
```

### Failure Catalog (002 fehler.txt)
```text
# Fehlercode;Beschreibung
5;Spannungsabfall
4;Kein Signal
3;Überhitzung
2;Sensor defekt
```

### Downtime Records (003 stillstand.txt)
```text
# MaschinenCode;FehlerCode;Startzeit;Endzeit
104;9;16.02.12 13:45;16.02.12 14:20
103;10;16.02.12 14:30;16.02.12 15:15
102;6;16.02.12 15:30;16.02.12 16:10
101;7;16.02.12 16:30;16.02.12 17:00
```

---

## Technical Implementation

### Core Data Structures (C)
```c
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
    char startZeit[20];
    char endZeit[20];
    Maschine* maschine;
    Fehler* fehler;
    struct Stillstand* next;
    struct Stillstand* prev;
} Stillstand;
```

---

## Communication Protocol

| Command Format                  | Example                                 |
|--------------------------------|-----------------------------------------|
| `ADD_MACHINE;code;name`        | `ADD_MACHINE;101;Fräsmaschine`          |
| `ADD_FAILURE;code;desc`        | `ADD_FAILURE;5;Spannungsabfall`         |
| `NEU;mcode;fcode;start;end`    | `NEU;101;5;16.02.12 10:00;16.02.12 10:30`|

Server responses:

- Success: `OK;operation_details`
- Error: `ERROR;reason`

---

## Development Notes


## Build Instructions (Windows)
```bash
# Compile server
cl TCP_server_data_base_caipo.c /link Ws2_32.lib

# Compile client
cl TCP_client_data_base_caipo.c /link Ws2_32.lib

# Execution sequence
1. Start server: TCP_server_data_base_caipo.exe
2. Connect client: TCP_client_data_base_caipo.exe
```

---

## License

Developed for educational purposes as part of coursework at Hochschule Furtwangen University.  
© 2023 Manuel Alberto Caipo Ccoa. All rights reserved.
