#include "../interface/FileExists.h"
#include <fstream>
#include <assert.h>

  /**
   * Will test whether a file exists
   * if fail is set to true, executable will exit
   */

bool fexists(const std::string & filename, bool fail)
{
  std::ifstream ifile(filename.c_str());
  if (fail && !ifile) {
    std::cout << "File does not exist: " << filename << std::endl;
    assert(ifile);
  }
  return ifile;
}

 
