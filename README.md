# N_PCA (主成分分析)  

## 说明  
使用PCA进行图像模板匹配的通用框架.  

## 开发环境 Environment  
Ubantu18.04  

## 依赖
```txt
OpenCV

#Eigen3
sudo apt-get install libeigen3-dev
```

## 使用   
**使用前请确保已配置好OpenCV**   
```shell  
git clone git@github.com:wxxhub/N_PCA.git  
cd N_PCA  
mkdir build  
cd build  
cmake ..  
make  

# process data
./demo  

# runresult
## finished load set...
## finished reduce dimension...
## finished process...
## start save...
## save data in ../data/orl_faces.txt
## process finished
## detector faile test index: 20
## train size: 362, test_size: 38, match_success: 37, recognition rate: 0.973684
## 20, s28


# use processed data to detector
./detector_demo  

# runresult
## finished read data
## 4, s13
```

## API说明
```cpp
void init(const std::string config_path) # 根据配置文件进行配置
void process()                           # 对数据进行处理

void readData(const std::string data_path) # 读取已经生产的数据, 以便进行识别

# 三种识别API, 可以使用路径, Mat, 和u_char*, 返回值为标签  
int detector(u_char* image_data);
int detector(cv::Mat image);
int detector(const std::string image_path);

# 解析label.txt生成标签及其对象名  
std::map<int, std::string> getLabelMap(const std::string label_path)

# 对与大小不符合图片进行resize, 先将图片等比压缩或者拉伸, 再将空余部分填充为平均值, 需要先将图片灰度化  
void n_resize(cv::Mat &image)
```

## Config文件说明  
#### config.txt  
```txt
image_width: 92    # 图片宽度
image_height: 112  # 图片高度
dimension: 40      # 降低到的维数
train_file_path: ../config/train.txt  # 训练集数据路径
test_file_path: ../config/test.txt    # 测试集路径
save_data_path: ../data/orl_faces.txt # 数据存储路径
```

#### label.txt  
标签及对应对象名称  

#### train.txt and test.txt  
训练集和测试集  

## data文件说明
```txt
dimension #数据维度
image_width #图片宽度
image_height #图片高度
train_mean #训练集的平均值
train_label #训练集的标签
train_data_matrix #训练集乘以基的新数据
basic_matrix #基
```

## 训练集说明
```txt
../image/orl_faces/s1/10.pgm 0 #图片路径及标签, 可以使用绝对路径和相对路径
```

## 创建自己的数据集
```txt
文件夹名为数据对象名称,如orl_faces文件下的s1, s2,对应名称为s1, s2  
使用script中的create_dataset.sh创建训练集, 将create_dataset.sh考到数据集目录, 例如image  
./create_dataset.sh orl_faces
会在当前路径生成 label.txt, train.txt, test.txt  
修改config中的路径及其他参数  
```