#include <iostream>
//#include <exception>
#include "diskexceptions.h"
#include "disksim.h"


int main(int argc, char *argv[]) {
    std::string _position;
    std::string _direction;
    std::string _inputFile;
    std::string _outputFile;
    int _pos = 0;
    int _dir = 0;
    //check for correct param length
    try{
        int args = argc;
        if(args != 5){
            throw NotEnoughParams(std::string("Need more parameters 'Position Direction inputfile outputfile'"));
        }else{
            _position = argv[1];
            _direction = argv[2];
            _inputFile = argv[3];
            _outputFile = argv[4];
        }
    }catch(const NotEnoughParams& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }
    //Position is between 0<->199 and no other value
    try {
        _pos = std::stoi(_position);
        if(_pos < 0 || _pos > 199){
            throw NumberFail(std::string("Position out of range 0<->199"));
        }
    }catch(const NumberFail& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }catch(const std::exception& e){
        std::cout<< e.what() << ", Invalid Input" << '\n';
        exit(0);
    }
    //Direction is either 0(down) or 1(up) and no other value
    try{
        _dir = std::stoi(_direction);
        if(_dir < 0 || _dir > 1){
            throw NumberFail(std::string("Direction must be 0(down) or 1(up)"));
        }
    }catch(const NumberFail& e){
        std::cout << e.what() << '\n';
        exit(0);
    }catch(const std::exception& e){
        std::cout << e.what() << ", Invalid Input" << '\n';
        exit(0);
    }

    //evaluate the data passed in.
    evaluate(_pos, _dir, _inputFile, _outputFile);
    return 0;
}