#include <iostream>
#include "memexceptions.h"
#include "memsim.h"


int main(int argc, char *argv[]) {
    std::string _frameSize;
    std::string _inputFile;
    std::string _outputFile;
    int _memorySize = 0;
    try{
        int args = argc;
        if(args != 4){
            throw NotEnoughParams(std::string("Need more parameters 'numberofpages inputfile outputfile'"));
        }else{
            _frameSize = argv[1];
            _inputFile = argv[2];
            _outputFile = argv[3];
        }
    }catch(const NotEnoughParams& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }

//make sure that the value of Number of frams is of 128 256 512 or 1024 and nothing else
    try {
        _memorySize = std::stoi(_frameSize);
        if (_memorySize < 1 || _memorySize > 10000){
            throw NoOfPageexception(std::string("Memory size imput error: range: 1 to 10000"));
        }
    }catch(const NoOfPageexception& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }
    //evaluate the data passed in.
    evaluate(_memorySize, _inputFile, _outputFile);
    return 0;
}