#include <stdio.h>

#include "n_pca/n_pca.h"

int main()
{
    n_pca::NPca *pca = new n_pca::NPca();
    pca->init("../config/config.txt");
    pca->process();
    printf("process finished\n");
    pca->test();

    int result = pca->detector("../image/orl_faces/s28/3.pgm");
    std::map<int, std::string> label_map = pca->getLabelMap("../config/label.txt");
    printf("%d, %s\n", result, label_map[result].c_str());
    return 0;
}