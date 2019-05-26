#include <stdio.h>

#include "n_pca/n_pca.h"

int main()
{
    auto pca = new n_pca::NPca();
    pca->init("../config/config.txt");
    pca->process();
    printf("process finished\n");
    pca->test();
    return 0;
}