/*
 * n_config.h
 *
 *  Created on: 2019. 5. 24.
 *      Author: zerom
 *      E-mail: wxx0520@foxmail.com
 */

#ifndef _N_CONFIG_H_
#define _N_CONFIG_H_

#include <iostream>

namespace n_pca
{

class NConfig
{
public:
    NConfig();
    ~NConfig();

    void init(const std::string config_path);
    std::vector<std::string> n_split(const std::string &text, char sep);

    int image_width_;
    int image_height_;

    int min_dimension_;
    int max_dimension_;

    std::string train_file_path_;
    std::string test_file_path_;
    std::string save_data_path_;

private:
    void split();
};

}

#endif /* _N_CONFIG_H_ */