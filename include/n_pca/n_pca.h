/*
 * n_pca.h
 *
 *  Created on: 2019. 5. 24.
 *      Author: zerom
 *      E-mail: wxx0520@foxmail.com
 */

#ifndef _N_PCA_H_
#define _N_PCA_H_

#include <opencv2/highgui/highgui.hpp>
#include <eigen3/Eigen/Dense>

#include "n_pca/n_config.h"

namespace n_pca
{

class NPca
{
public:
    NPca();
    ~NPca();

    void init(const std::string config_path);
    void process();
    void test();

    // read available data
    void readData(const std::string data_path);

    // detector and get detector result
    bool detector(cv::Mat image, std::vector<float, int>& result);

private:
    NConfig *n_config_;

    std::vector<int> train_lab_;
    std::vector<int> test_lab_;
    
    Eigen::MatrixXd train_matrix_;
    Eigen::MatrixXd test_matrix_;

    Eigen::MatrixXd train_mean_;
    Eigen::MatrixXd train_data_matrix_;
    Eigen::MatrixXd basic_matrix_;

    // set config file
    void setTrainMatrix(const std::string train_config_path);
    void setTestMatrix(const std::string test_config_path);

    // save_data
    void saveData();
};

}

#endif /* _N_PCA_H_ */