#include <stdio.h>
#include <map>

#include "n_pca/n_pca.h"

int main()
{
    n_pca::NPca *pca = new n_pca::NPca();
    pca->readData("../data/orl_faces.txt");

    printf("finished read data\n");

    int result = pca->detector("../image/orl_faces/s13/1.pgm");
    std::map<int, std::string> label_map = pca->getLabelMap("../config/label.txt");
    printf("%d, %s\n", result, label_map[result].c_str());
    return 0;
}