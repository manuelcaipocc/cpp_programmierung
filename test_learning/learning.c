#include <stdio.h>


// file copying
// main()
// {
//     int c;
//     c = getchar();
//     while (c != EOF) {
//         putchar(c);
//         c = getchar();
//     }
// }


// main()
// {
//     int c;
//     printf("Value of EOF: %d\n",EOF);
//     while ((c=getchar())!=EOF)
//         putchar(c);   
// }

// main()
// {
//     long nc;
//     nc = 0;
//     while (getchar() != EOF)
//         ++nc;
//     printf("%ld\n", nc);
// }

// test para revisar codigo 

// int main() {
//     long nc = 0;
//     int c;

//     printf("Empieza a escribir (Ctrl+Z + Enter para terminar):\n");

//     while ((c = getchar()) != EOF)
//         ++nc;

//     printf("Número de caracteres: %ld\n", nc);

//     // Agrega pausa si ejecutas en Visual Studio para ver el resultado
//     printf("Presiona Enter para salir...");
//     getchar();

//     return 0;
// }

// counting words and much more
 
#define IN 1
#define OUT 0

// EOF end of file
int main()
{
    int c,nl,nw,nc, state;

    state =OUT;
    c=nl=nw=nc=0;
    //'H' → 'o' → 'l' → 'a' → '\n' → EOF. con get char el usuaio ingresa un parametro que sera analizado
    while ((c=getchar()) != EOF) {
        ++nc;
        if (c=='\n')
            ++nl;
        if (c==' '|| c=='\t' || c=='\n')
            state=OUT;
        else if (state==OUT) {
            state=IN;
            ++nw;
        }
    }
    printf("Lineas %d palabras %d caracteres %d\n", nl, nw, nc);
    return 0;
}

