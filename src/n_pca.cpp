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
    delete n_config_;
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
    MatrixXd PC = eigenSolver.eigenvectors().rightCols(n_config_->dimension_);
    printf("finished reduce dimension...\n");

    basic_matrix_ = train_zero_matrix.transpose() * PC;

    MatrixXd basic_squre_matrix = basic_matrix_.array().square();

    // normalization
    for (int i = 0; i < basic_matrix_.cols(); i++)
    {
        basic_matrix_.col(i) = basic_matrix_.col(i)/sqrt(basic_squre_matrix.col(i).sum());
    }

    train_data_matrix_ = train_zero_matrix * basic_matrix_;

    printf("finished process...\n");

    saveData();
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
        }

        if (test_label_[i] == train_label_[match_index])
        {
            match_times++;
        }
        else
        {
            printf("detector faile test index: %d\n", i);
        }
        
    }

    printf("train size: %d, test_size: %d, match_success: %d, recognition rate: %f\n", train_matrix_.rows(), test_matrix_.rows(), match_times, 1.0 * match_times / test_matrix_.rows());
}

void NPca::readData(const string data_path)
{
    if (data_path.length() <= 0)
    {
        printf("couldn't find data_path\n");
        return;
    }

    ifstream file(data_path.c_str());

    if (!file.is_open())
    {
        printf("couldn't open data_path: %s\n", data_path.c_str());
        return;
    }

    string input_str;
    while(getline(file, input_str))
    {
        vector<string> tokens = n_config_->n_split(input_str, ':');

        // set value
        if (tokens[0] == "dimension")
        {
            n_config_->dimension_ = atoi(tokens[1].c_str());
        }
        if (tokens[0] == "image_width")
        {
            n_config_->image_width_ = atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "image_height")
        {
            n_config_->image_height_ = atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "train_mean")
        {
            vector<string> values = n_config_->n_split(input_str, ' ');
            vector<double> mean_vector;
            for (int i = 1; i < values.size(); i++)
            {
                if (values[i] == "")
                {
                    continue;
                }
                mean_vector.push_back(atof(values[i].c_str()));
            }

            train_mean_ = MatrixXd::Zero(1, mean_vector.size());

            for (int i = 0; i < mean_vector.size(); i++)
                train_mean_.coeffRef(0, i) = mean_vector[i];
        }
        else if (tokens[0] == "train_label")
        {
            vector<string> values = n_config_->n_split(input_str, ' ');
            for (int i = 0; i < values.size(); i++)
            {
                if (values[i] == "")
                    continue;

                train_label_.push_back(atoi(values[i].c_str()));
            }
        }
        else if (tokens[0] == "train_data_matrix")
        {
            vector<string> row = n_config_->n_split(tokens[1].c_str(), ',');
            vector<vector<double> > train_data_vector;
            for (int i = 0; i < row.size(); i++)
            {
                vector<double> values_vector;
                vector<string> values = n_config_->n_split(row[i], ' ');
                for (int j = 0; j < values.size(); j++)
                {
                    if (values[j] == "")
                    {
                        continue;
                    }
                    values_vector.push_back(atof(values[j].c_str()));
                }
                if (values_vector.size() > 0)
                    train_data_vector.push_back(values_vector);
            }

            train_data_matrix_ = MatrixXd::Zero(train_data_vector.size(), train_data_vector[0].size());

            for (int i = 0; i < train_data_vector.size(); i++)
            {
                for (int j = 0; j < train_data_vector[i].size(); j++)
                {
                    train_data_matrix_.coeffRef(i, j) = train_data_vector[i][j];
                }
            }
        }
        else if (tokens[0] == "basic_matrix")
        {
            vector<string> row = n_config_->n_split(tokens[1].c_str(), ',');
            vector<vector<double> > basic_vector;
            for (int i = 0; i < row.size(); i++)
            {
                vector<double> values_vector;
                vector<string> values = n_config_->n_split(row[i], ' ');
                for (int j = 0; j < values.size(); j++)
                {
                    if (values[j] == "")
                    {
                        continue;
                    }
                    values_vector.push_back(atof(values[j].c_str()));  
                }
                if (values_vector.size() > 0)
                    basic_vector.push_back(values_vector);
            }

            basic_matrix_ = MatrixXd::Zero(basic_vector.size(), basic_vector[0].size());

            for (int i = 0; i < basic_vector.size(); i++)
            {
                for (int j = 0; j < basic_vector[i].size(); j++)
                {
                    basic_matrix_.coeffRef(i, j) = basic_vector[i][j];
                }
            }
        }
    }

}

int NPca::detector(u_char* image_data)
{
    int size = n_config_->image_height_ * n_config_->image_width_;  
    MatrixXd detector_matrix = MatrixXd::Zero(1, size);

    for (int i = 0; i < size; i++)
        detector_matrix.coeffRef(0, i) = image_data[i];

    MatrixXd detector_zero_matrix = detector_matrix;

    detector_zero_matrix.row(0) -= train_mean_.row(0);

    MatrixXd detector_data_matrix = detector_zero_matrix * basic_matrix_;
    int match_index = 0;
    double min_difference = -1;
    for (int i = 0; i < train_data_matrix_.rows(); i++)
    {
        double difference = (detector_data_matrix.row(0) - train_data_matrix_.row(i)).squaredNorm();
        if (min_difference < 0 || min_difference > difference)
        {
            min_difference = difference;
            match_index = i;
        }
    }

    return train_label_[match_index];
}

int NPca::detector(Mat image)
{
    if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
    {
        n_resize(image);
    }

    image = image.reshape(0,1);
    
    return detector(image.data);
}

int NPca::detector(const std::string image_path)
{
    if (image_path.length() <= 0)
    {
        printf("couldn't find image_path\n");
        return -1;
    }

    Mat image = imread(image_path, IMREAD_GRAYSCALE);

    if (image.empty())
    {
        printf("open image failde!\n");
        return -1;
    }
    return detector(image);
}

void NPca::setTrainMatrix(const string train_config_path)
{
    if (train_config_path.length() <= 0)
    {
        printf("couldn't find train_config_path\n");
        return;
    }

    ifstream file(train_config_path.c_str());

    if (!file.is_open())
    {
        printf("couldn't open train_config_path: %s\n", train_config_path.c_str());
        return;
    }

    string input_str;
    int count = 0;
    int size = n_config_->image_height_ * n_config_->image_width_;
    vector<Mat> image_data_;

    while (getline(file, input_str))
    {
        vector<string> read_data = n_config_->n_split(input_str, ' ');
        if (read_data.size() < 2)
            continue;
        Mat image;
        try{
            image = imread(read_data[0].c_str(), IMREAD_GRAYSCALE);
            if (image.empty())
                continue;
        } catch (Exception e) {
            continue;
        }

        // n_resize
        if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
        {
            n_resize(image);
        }

        Mat reshap_image = image.reshape(0, 1);
        image_data_.push_back(reshap_image);
        train_label_.push_back(atoi(read_data[1].c_str()));
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

    ifstream file(test_config_path.c_str());

    if (!file.is_open())
    {
        printf("couldn't open test_config_path: %s\n", test_config_path.c_str());
        return;
    }

    string input_str;
    int count = 0;
    int size = n_config_->image_height_ * n_config_->image_width_;
    vector<Mat> image_data_;

    while (getline(file, input_str))
    {
        vector<string> read_data = n_config_->n_split(input_str, ' ');

        if (read_data.size() < 2)
            continue;
        
        Mat image;
        try{
            image = imread(read_data[0].c_str(), IMREAD_GRAYSCALE);
            
            if (image.empty())
                continue;

        } catch (Exception e) {
            continue;
        }

        // n_resize
        if (image.cols != n_config_->image_width_ || image.rows != n_config_->image_height_)
        {
            n_resize(image);
        }

        Mat reshap_image = image.reshape(0, 1);
        image_data_.push_back(reshap_image);
        test_label_.push_back(atoi(read_data[1].c_str()));
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
    printf("start save...\n");
    if (n_config_->save_data_path_.size() < 1)
    {
        printf("couldn't find save path!\n");
        return;
    }

    ofstream file(n_config_->save_data_path_.c_str());
    
    if (!file.is_open())
    {
        printf("open file failed!\n");
        return;
    }

    file.clear();
    file<<"dimension: "<<n_config_->dimension_<<endl;
    file<<"image_width: "<<n_config_->image_width_<<endl;
    file<<"image_height: "<<n_config_->image_height_<<endl;
    file<<"train_mean: "<<train_mean_<<endl;

    file<<"train_label: ";
    for (int i = 0; i < train_label_.size(); i++)
        file<<train_label_[i]<<" ";
    file<<endl;
    
    file<<"train_data_matrix: ";
    for (int i = 0; i < train_data_matrix_.rows(); i++)
        file<<train_data_matrix_.row(i)<<",";
    file<<endl;

    file<<"basic_matrix:";
    for (int i = 0; i < basic_matrix_.rows(); i++)
        file<<basic_matrix_.row(i)<<",";
    file<<endl;

    file.close();

    printf("save data to %s\n", n_config_->save_data_path_.c_str());
}

map<int, string> NPca::getLabelMap(const string label_path)
{
    map<int, string> label_map;

    if (label_path.length() <= 0)
    {
        printf("couldn't find label_path\n");
        return label_map;
    }

    ifstream file(label_path.c_str());

    if (!file.is_open())
    {
        printf("couldn't open label_path: %s\n", label_path.c_str());
        return label_map;
    }

    string input_str;
    while (getline(file, input_str))
    {
        vector<string> tokens = n_config_->n_split(input_str, ':');
        label_map[atoi(tokens[0].c_str())] = tokens[1];
    }

    return label_map;
}

void NPca::n_resize(cv::Mat &image)
{
    Mat process_image(n_config_->image_height_, n_config_->image_width_, image.type(), mean(image)(0));
    if (1.0 * image.cols / image.rows < 1.0 * n_config_->image_width_ / n_config_->image_height_)
    {
        /*
        *       ##########          ############################
        *       #        #          #                          #
        *       #  image #          #      n_config image      #
        *       ##########          ############################
        */

        int strech_width = n_config_->image_height_ * image.cols / image.rows;
        resize(image, image, Size(strech_width, n_config_->image_height_));
        int pandin = (n_config_->image_width_ - strech_width)/2;
        Mat center_image = process_image(Rect(pandin, 0, strech_width, n_config_->image_height_));
        image.copyTo(center_image);
    }
    else if (1.0 * image.cols / image.rows > 1.0 * n_config_->image_width_ / n_config_->image_height_)
    {
        /*
        *       #################          ##############################
        *       #               #          #                            #
        *       #n_config image #          #      image                 #
        *       #################          ##############################
        */

        int strech_height = n_config_->image_width_ * image.rows / image.cols;
        resize(image, image, Size(n_config_->image_width_, strech_height));
        int pandin = (n_config_->image_height_ - strech_height)/2;
        Mat center_image = process_image(Rect(0, pandin, n_config_->image_width_, strech_height));
        image.copyTo(center_image);
    }
    else
    {
        resize(image, process_image, Size(n_config_->image_width_, n_config_->image_height_));
    }

    image = process_image.clone();
}