#include <stdio.h>

// main(){
//     printf("hello,HFU world\n");
// }

main()
{
    int fahr,celcius;
    int lower,upper,step;

    lower=0;
    upper=300;
    step=20;

    fahr=lower;
    while (fahr<upper)
    {
        celcius=5*(fahr-32)/9;
        printf("%d \t %d \n",fahr,celcius);
        fahr=fahr+step;
    }
    return 0;
}