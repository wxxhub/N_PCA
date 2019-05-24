#include <fstream>
#include <algorithm>
#include <vector>

#include "n_pca/n_config.h"

using namespace std;
using namespace n_pca;

// split functions
static inline string &ltrim(string &s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}
static inline string &rtrim(string &s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}
static inline string &trim(string &s)
{
  return ltrim(rtrim(s));
}

// NConfig
NConfig::NConfig()
{
    
}

NConfig::~NConfig()
{

}

void NConfig::init(const string config_path)
{
    ifstream file(config_path.c_str());

    if (!file.is_open())
    {
        printf("Open Config Failed!\n");
    }

    while (!file.eof())
    {
        string line_str;
        getline(file, line_str);

        vector<string> tokens = n_split(line_str, ':');

        // set value
        if (tokens[0] == "image_width")
        {
          image_width_ = std::atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "image_height")
        {
          image_height_ = std::atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "min_dimension")
        {
          min_dimension_ = std::atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "max_dimension")
        {
          max_dimension_ = std::atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "min_dimension")
        {
          min_dimension_ = std::atoi(tokens[1].c_str());
        }
        else if (tokens[0] == "train_file_path")
        {
          train_file_path_ = tokens[1];
        }
        else if (tokens[0] == "test_file_path")
        {
          test_file_path_ = tokens[1];
        }
        else if (tokens[0] == "save_data_path")
        {
          save_data_path_ = tokens[1];
        }
    }
}

vector<string> NConfig::n_split(const string &text, char sep)
{
  vector<string> tokens;
  size_t start = 0, end = 0;

  while ((end = text.find(sep, start)) != (string::npos))
  {
    tokens.push_back(text.substr(start, end - start));
    trim(tokens.back());
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  trim(tokens.back());

  return tokens;
}