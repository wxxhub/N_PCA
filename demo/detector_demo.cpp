#include <stdio.h>

#include "n_pca/n_pca.h"

int main()
{
    auto pca = new n_pca::NPca();
    pca->readData("../data/orl_faces.txt");
    int result = pca->detector("../image/orl_faces/s13/1.pgm");
    auto label_map = pca->getLabelMap("../config/label.txt");
    printf("%d, %s\n", result, label_map[result].c_str());
    return 0;
}