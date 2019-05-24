#include <fstream>
#include "n_pca/n_pca.h"

using namespace std;
using namespace cv;
using namespace n_pca;
using namespace Eigen;

NPca::NPca()
{
    n_config_ = new NConfig();
}

NPca::~NPca()
{

}

void NPca::init(const string config_path)
{
    n_config_->init(config_path);
}

void NPca::train()
{
    setTrainMatrix(n_config_->train_file_path_);
    setTestMatrix(n_config_->test_file_path_);

    cout<<train_matrix_<<endl;
}

void NPca::readData(const string data_path)
{

}

bool NPca::detector(Mat image, vector<float, int>& result)
{

}

void NPca::setTrainMatrix(const string train_config_path)
{
    if (train_config_path.length() <= 0)
    {
        printf("couldn't find train_config_path\n");
        return;
    }

    ifstream file(train_config_path);

    if (!file.is_open())
    {
        printf("couldn't find train_config_path: %s\n", train_config_path.c_str());
        return;
    }

    string input_str;
    int count = 0;
    int size = n_config_->image_height_ * n_config_->image_width_;
    vector<u_char*> image_data_;

    while (getline(file, input_str))
    {
        vector<std::string> read_data = n_config_->n_split(input_str, ' ');

        if (read_data.size() < 2)
            continue;
        
        Mat image;
        try{
            image = imread(read_data[0].c_str(), CV_LOAD_IMAGE_ANYDEPTH);
            
            if (image.empty())
                continue;

        } catch (Exception e) {
            continue;
        }

        if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
        {

        }

        Mat reshap_image = image.reshape(0, 1);
        image_data_.push_back(reshap_image.data);
        train_lab_.push_back(atoi(read_data[1].c_str()));
    }
    
    // set train_matrix
    train_matrix_ = MatrixXf::Zero(image_data_.size(), size);

    for (int i = 0; i < image_data_.size(); i++)
    {
        for (int j = 0; j < size; j++)
        {
            train_matrix_.coeffRef(i, j) = float(image_data_[i][j]);
        }
    }
}

void NPca::setTestMatrix(const string test_config_path)
{
    if (test_config_path.length() <= 0)
    {
        printf("couldn't find test_config_path\n");
        return;
    }

    ifstream file(test_config_path);

    if (!file.is_open())
    {
        printf("couldn't find test_config_path: %s\n", test_config_path.c_str());
        return;
    }

    string input_str;
    int count = 0;
    int size = n_config_->image_height_ * n_config_->image_width_;
    vector<u_char*> image_data_;

    while (getline(file, input_str))
    {
        vector<std::string> read_data = n_config_->n_split(input_str, ' ');

        if (read_data.size() < 2)
            continue;
        
        Mat image;
        try{
            image = imread(read_data[0].c_str(), CV_LOAD_IMAGE_ANYDEPTH);
            
            if (image.empty())
                continue;

        } catch (Exception e) {
            continue;
        }

        if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
        {
            
        }

        Mat reshap_image = image.reshape(0, 1);
        image_data_.push_back(reshap_image.data);
        test_lab_.push_back(atoi(read_data[1].c_str()));
    }
    
    // set test_matrix
    test_matrix_ = MatrixXf::Zero(image_data_.size(), size);

    for (int i = 0; i < image_data_.size(); i++)
    {
        for (int j = 0; j < size; j++)
        {
            test_matrix_.coeffRef(i, j) = float(image_data_[i][j]);
        }
    }
}

void NPca::saveData()
{

}