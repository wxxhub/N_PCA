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
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include <map>

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
    int detector(u_char* image_data);
    int detector(cv::Mat image);
    int detector(const std::string image_path);

    std::map<int, std::string> getLabelMap(const std::string label_path);

    // tool function
    void n_resize(cv::Mat &image);

private:
    NConfig *n_config_;

    std::vector<int> train_label_;
    std::vector<int> test_label_;
    
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