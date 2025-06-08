#include <stdio.h>


// int main()
// {   
//     float s1=1.0;
//     int t=10,b1,d1;
//     b1=t-'0';
//     d1=t-'1';
//     printf("test %d \n",t);
//     printf("review %d \n",d1);
//     printf("review %d \n",b1);
//     return 0;
// }

int main()
{
    int c,i,nwhite,nother;
    int *c_estrella=&c;
    // int ndigit[10];
    int ndigit[10]={0}; // inicializa el primer valor a el vlaor especificado
    // y a los siguientes vlaores lo inicia con cero
    // printf("c: %d i: %d , endwhite: %d, nother: %d ",c,i,nwhite,nother);
    nwhite=nother=c=i=1234;

    // for (i=0;i<10;++i)
    //     ndigit[i]=1;

    printf("c: %d i: %d ,endwhite: %d, nother: %d , ndigit: %d \n",c,i,nwhite,nother,ndigit[1]);
    //memset(&c,0,sizeof(c)); // limpia la memoria de la varbiale 

    printf("probemos *c: %d \n",*c_estrella);
    printf("probemos &c: %d \n",&c);
    return 0;
}