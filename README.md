# N_PCA

## 开发环境 Environment
Ubantu18.04  

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

## Config文件说明  
#### config.txt  
```txt
image_width: 92   
image_height: 112  
dimension: 40  
train_file_path: ../config/train.txt  
test_file_path: ../config/test.txt  
save_data_path: ../data/orl_faces.txt  
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
> 文件夹名为数据对象名称,如orl_faces文件下的s1, s2,对应名称为s1, s2  
> 使用script中的create_dataset.sh创建训练集, 将create_dataset.sh考到数据集目录, 例如image  
> ./create_dataset.sh orl_faces
> 会在当前路径生成 label.txt, train.txt, test.txt  
> 修改config中的路径及其他参数  