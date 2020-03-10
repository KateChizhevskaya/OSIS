#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;


int main(int argc, char *argv[])
{

          std::string prefix, filename;
          std::cout << "print file name without extension" << "\n";
          std::cin >> prefix;
          std::cout << "print result file name without extension" << "\n";
          std::cin >> filename;
          std::ofstream out(filename, std::ios_base::app);
          std::string path = "./";
          for (const auto &p : fs::directory_iterator(path))
          {
                  const fs::path filepath = p.path();
                  const fs::path fileName = filepath.filename();
                  std::string stringFileName = fileName.u8string();
                  if (stringFileName.substr(0, prefix.length()) == prefix) {
                          std::ifstream in(fileName);
                          std::string s;
                          while (getline(in, s))
                          {
                                  out << s << " ";
                          }
                  }
          }
          return 0;
  }
