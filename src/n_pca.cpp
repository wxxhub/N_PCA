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

void NPca::process()
{
    setTrainMatrix(n_config_->train_file_path_);
    setTestMatrix(n_config_->test_file_path_);

    printf("finished load set...\n");

    train_mean_ = train_matrix_.colwise().mean();

    RowVectorXd mean_vec_row(RowVectorXd::Map(train_mean_.data(), train_matrix_.cols()));

    MatrixXd train_zero_matrix = train_matrix_;
    train_zero_matrix.rowwise() -= mean_vec_row;

    // calculate covariance
    MatrixXd C = train_zero_matrix * train_zero_matrix.transpose();

    // calculate feature vector and value
    Eigen::SelfAdjointEigenSolver<MatrixXd> eigenSolver(C);

    // reduce dimension
    MatrixXd PC = eigenSolver.eigenvectors().rightCols(n_config_->min_dimension_);
    printf("finished reduce dimension...\n");

    basic_matrix_ = train_zero_matrix.transpose() * PC;

    MatrixXd basic_squre_matrix = basic_matrix_.array().square();

    // normalization
    for (int i = 0; i < basic_matrix_.cols(); i++)
    {
        basic_matrix_.col(i) = basic_matrix_.col(i)/sqrt(basic_squre_matrix.col(i).sum());
    }

    train_data_matrix_ = train_zero_matrix * basic_matrix_;
}

void NPca::test()
{
    RowVectorXd mean_vec_row(RowVectorXd::Map(train_mean_.data(), test_matrix_.cols()));

    MatrixXd test_zero_matrix = test_matrix_;
    test_zero_matrix.rowwise() -= mean_vec_row;

    MatrixXd test_data_matrix = test_zero_matrix * basic_matrix_;

    int match_times = 0;

    for (int i = 0; i < test_data_matrix.rows(); i++)
    {
        double min_difference = -1;
        int match_index = 0;
        for (int j = 0; j < train_data_matrix_.rows(); j++)
        {
            double difference = (test_data_matrix.row(i) - train_data_matrix_.row(j)).squaredNorm();
            if (min_difference < 0 || min_difference > difference)
            {
                // printf("min_difference: %f, difference: %f\n", min_difference, difference);
                min_difference = difference;
                match_index = j;
            }
            // printf("min_difference: %f, difference: %f\n", min_difference, difference);
        }

        if (test_lab_[i] == train_lab_[match_index])
        {
            match_times++;
        }
    }

    printf("train size: %d, test_size: %d, match_success: %d, recognition rate: %f\n", train_matrix_.rows(), test_matrix_.rows(), match_times, 1.0 * match_times / test_matrix_.rows());
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
    vector<Mat> image_data_;

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

        // resize
        if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
        {

        }

        Mat reshap_image = image.reshape(0, 1);
        image_data_.push_back(reshap_image);
        train_lab_.push_back(atoi(read_data[1].c_str()));
    }
    // set train_matrix
    train_matrix_ = MatrixXd::Zero(image_data_.size(), size);

    for (int i = 0; i < image_data_.size(); i++)
    {
        for (int j = 0; j < size; j++)
        {
            train_matrix_.coeffRef(i, j) = image_data_[i].data[j];
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
    vector<Mat> image_data_;

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

        // resize
        if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
        {
            
        }

        Mat reshap_image = image.reshape(0, 1);
        image_data_.push_back(reshap_image);
        test_lab_.push_back(atoi(read_data[1].c_str()));
    }
    
    // set test_matrix
    test_matrix_ = MatrixXd::Zero(image_data_.size(), size);

    for (int i = 0; i < image_data_.size(); i++)
    {
        for (int j = 0; j < size; j++)
        {
            test_matrix_.coeffRef(i, j) = image_data_[i].data[j];
        }
    }
}

void NPca::saveData()
{

}