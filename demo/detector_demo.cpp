#include <stdio.h>

#include "n_pca/n_pca.h"

int main()
{
    auto pca = new n_pca::NPca();
    pca->readData("../data/orl_faces.txt");
    return 0;
}