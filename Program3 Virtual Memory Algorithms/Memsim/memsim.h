//
// Created by Chase Brown on 11/19/2017.
//

#ifndef MEMSIM_MEMSIM_H
#define MEMSIM_MEMSIM_H

#include <iostream>
#include <fstream>
#include <exception>
#include <utility>
#include <iomanip>
#include <vector>
#include <cctype>
#include "memexceptions.h"

struct Proc{
    int _pValue = 0; //process reference value ex 4000
    int _arrivalTime = 0;       //place in que for proc
    int _pCount = 0;            //used for counts in difference ways for different algorithms.

};
void readFile(std::vector<Proc> &, const std::string &);
void evaluate(int, const std::string &, std::string);
void FIFOalg(std::vector<Proc> &, int, float &, float &, float &, float &, float &, float &);
void LFUalg(std::vector<Proc> &list, int _frameSize, float &, float &, float &, float &, float &, float &);
void LRUalg(std::vector<Proc> &list, int _frameSize, float &, float &, float &, float &, float &, float &);
void OPTalg(std::vector<Proc> &list, int _frameSize, float &, float &, float &, float &, float &, float &);
bool inMemory(Proc &,std::vector<Proc> &);
int lowestCount(std::vector<Proc> &);
int firstArrival(std::vector<Proc> &);
int highestCount(std::vector<Proc> &);
int findNextOccur(int , int, std::vector<Proc> &);
int longestVect(std::vector<Proc> &);
//evaluate the data.
void evaluate(int _frameSize, const std::string &_inFile, std::string _outFile){
    float _twoK = 0;
    float _fourK = 0;
    float _sixK = 0;
    float _eightK = 0;
    float _tenK = 0;
    float _totalPF = 0;
    //open file to print to.
    std::ofstream outFile(_outFile);
    try{
        if(!outFile){
            throw LoadException(std::string("OutPut File Failed to open"));
        }
    }catch(const LoadException& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }

    //create a vector to hold a list of process references.
    std::vector<Proc> _processList;
    _processList.emplace_back();

    //readfile will read the contents of the file and place them into the process list vector.
    readFile(_processList, _inFile);
    //remove the initial value pushed into the vector.
    _processList.erase(_processList.begin());

    outFile << "=================================================================" << '\n';
    outFile << "    Page Replacement Algorithm Simulation ( Frame Size: "<< _frameSize << " )" << '\n';
    outFile << "=================================================================" << '\n';
    outFile << "                                    Page Fault Rates" << '\n';
    outFile << "Algorithm  Total Page Faults   2000   4000   6000   8000   10000" << '\n';
    //call FIFO and print results.
    FIFOalg(_processList, _frameSize, _twoK, _fourK, _sixK, _eightK, _tenK, _totalPF);
    outFile << std::setprecision(4) << "FIFO"<< std::setw(11) << _totalPF << std::setprecision(3) << std::setw(20) << _twoK << std::setw(7) << _fourK << std::setw(7) << _sixK
              << std::setw(7) << _eightK << std::setw(8) << _tenK << '\n';
    _twoK = 0;
    _fourK = 0;
    _sixK = 0;
    _eightK = 0;
    _tenK = 0;
    //call LRU and print results
    LRUalg(_processList, _frameSize, _twoK, _fourK, _sixK, _eightK, _tenK, _totalPF);
    outFile << std::setprecision(4) << "LRU"<< std::setw(12)<< _totalPF << std::setprecision(3) << std::setw(20) << _twoK << std::setw(7) << _fourK << std::setw(7) << _sixK
              << std::setw(7) << _eightK << std::setw(8) << _tenK << '\n';
    _twoK = 0;
    _fourK = 0;
    _sixK = 0;
    _eightK = 0;
    _tenK = 0;
    //call LFU and print results
    LFUalg(_processList, _frameSize, _twoK, _fourK, _sixK, _eightK, _tenK, _totalPF);
    outFile << std::setprecision(4) << "LFU"<< std::setw(12)<< _totalPF << std::setprecision(3) << std::setw(20) << _twoK << std::setw(7) << _fourK << std::setw(7) << _sixK
              << std::setw(7) << _eightK << std::setw(8) << _tenK << '\n';
    _twoK = 0;
    _fourK = 0;
    _sixK = 0;
    _eightK = 0;
    _tenK = 0;
    //call Opt and print results.
    OPTalg(_processList, _frameSize, _twoK, _fourK, _sixK, _eightK, _tenK, _totalPF);
    outFile << std::setprecision(4) << "OPTIMAL"<< std::setw(8)<< _totalPF << std::setprecision(3) << std::setw(20) << _twoK << std::setw(7) << _fourK << std::setw(7) << _sixK
              << std::setw(7) << _eightK << std::setw(8) << _tenK << '\n';

    outFile.close();
}
//pass the vector by reference to be loaded with the processes in the input file
void readFile(std::vector<Proc> &list, const std::string &file){
    std::ifstream _infile(file);
    int count = 1;
    //try to read the file
    try{
        if(!_infile){
            throw LoadException(std::string("FileLoad Error"));
        }
    }catch(const LoadException& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }
    //read the file and getline each process.  Convert into a number for easier comparison later
    //Save arrival time
    //error check to make sure the value of the number is in the reference stream of 0-4095.
    while(!_infile.eof()){
        Proc number = Proc();
        std::string line;
        std::getline(_infile, line);
        if(!line.empty()){
            try {
                number._pValue = std::stoi(line);
                if(number._pValue < 0 || number._pValue > 4095){
                    std::cout << "Reference Error: " <<number._pValue;
                    throw NumberFail(std::string(", Reference stream range error: range: 0-4095"));
                }
            }catch(const NumberFail& e){
                std::cout << e.what() << '\n';
                exit(0);
            }catch(const std::exception& e){
                std::cout << e.what() << " Failed, bad input" << '\n';
                exit(0);
            }
            number._arrivalTime = count;
            count++;
            list.push_back(number);
        }
    }
}
//FIFO Alg "Lowest Arrival time gets booted from the memory if memory is full and cant hold process
void FIFOalg(std::vector<Proc> &list, int _frameSize, float &twoK, float &fourK, float &sixK, float &eightK, float &tenK, float &totalPF){
    std::vector<Proc> _memory;
    float _pageFault = 0;
    int _kickOut = 0;
    bool _inmemory;
    for(auto i : list){
        //if the process is not in memory pagefault
        _inmemory = inMemory(i,_memory);
        if(!_inmemory){
            _pageFault++;
            //if the memory is not full push into memory
            if(_memory.size() < _frameSize){
                _memory.push_back(i);
            }else{
                //search for FI
                _kickOut = firstArrival(_memory);
                //kickout holds the process to be removed.
                //replace
                _memory[_kickOut-1]._pValue = i._pValue;
                _memory[_kickOut-1]._arrivalTime = i._arrivalTime;
                _memory[_kickOut-1]._pCount = i._pCount;
            }
        }
        //calculate results at different faults
        if(i._arrivalTime == 2000) {
            twoK = _pageFault/2000;
        }
        if(i._arrivalTime == 4000){
            fourK = _pageFault/4000;
        }
        if(i._arrivalTime == 6000){
            sixK = _pageFault/6000;
        }
        if(i._arrivalTime == 8000){
            eightK = _pageFault/8000;
        }
        if(i._arrivalTime == 10000){
            tenK = _pageFault/10000;
        }
    }
    totalPF = _pageFault;
}
//LFU The process with the least frequency is replaced if memory is full.  If there is a tie then FIFO between the tie to break
void LFUalg(std::vector<Proc> &list, int _frameSize, float &twoK, float &fourK, float &sixK, float &eightK, float &tenK, float &totalPF){
    std::vector<Proc> _memory;
    float _pageFault = 0;
    int _kickOut = 0;
    bool _inmemory;
    for(auto i : list){
        _inmemory = inMemory(i, _memory);
        if(!_inmemory){
            _pageFault++;
            //if the memory is not full push into memory
            if(_memory.size() < _frameSize){
                i._pCount++;
                _memory.push_back(i);
                //else search for the process with least usuage "smallest Pcount"
                //if there is a tie then use FIFO for tie breaker.
            }else{
                //search for the process with lowest count.
                //replace
                _kickOut = lowestCount(_memory);
                _memory[_kickOut]._pValue = i._pValue;
                _memory[_kickOut]._arrivalTime = i._arrivalTime;
                _memory[_kickOut]._pCount = ++i._pCount;
            }
            //else push into memory
        }else{
            int pos = 0;
            int count = 0;
            for(auto j : _memory){
                if(j._pValue == i._pValue){
                    pos = count;
                }
                count++;
            }
            _memory[pos]._pCount++;
        }
        //calculate results at different faults
        if(i._arrivalTime == 2000) {
            twoK = _pageFault/2000;
        }
        if(i._arrivalTime == 4000){
            fourK = _pageFault/4000;
        }
        if(i._arrivalTime == 6000){
            sixK = _pageFault/6000;
        }
        if(i._arrivalTime == 8000){
            eightK = _pageFault/8000;
        }
        if(i._arrivalTime == 10000){
            tenK = _pageFault/10000;
        }
    }
    totalPF = _pageFault;
}
//LRU The process with least reference is replaced when memory is full.  If ther eis a tie then FIFO between tie to break
void LRUalg(std::vector<Proc> &list, int _frameSize, float &twoK, float &fourK, float &sixK, float &eightK, float &tenK, float &totalPF){
    std::vector<Proc> _memory;
    float _pageFault = 0;
    int _kickOut = 0;
    bool _inmemory;
    for(auto i : list){
        _inmemory = inMemory(i, _memory);
        if(!_inmemory){
            _pageFault++;
            //if the memory is not full push into memory
            if(_memory.size() < _frameSize){
                for(int k = 0; k < _memory.size(); ++k){
                    _memory[k]._pCount++;
                }
                i._pCount++;
                _memory.push_back(i);
                //else if the memory is full search for process with highest time
                //replace
            }else{
                _kickOut = highestCount(_memory);
                _memory[_kickOut]._pValue = i._pValue;
                _memory[_kickOut]._arrivalTime = i._arrivalTime;
                _memory[_kickOut]._pCount = ++i._pCount;
            }
            //else the process is already in memory increment values
        }else{
            int pos = 0;
            int count = 0;
            for(int k = 0; k < _memory.size(); ++k){
                _memory[k]._pCount++;
            }
            for(auto j : _memory){
                if(j._pValue == i._pValue){
                    pos = count;
                }
                count++;
            }
            _memory[pos]._pCount = 1;
        }
        //calculate results at differnet faults
        if(i._arrivalTime == 2000) {
            twoK = _pageFault/2000;
        }
        if(i._arrivalTime == 4000){
            fourK = _pageFault/4000;
        }
        if(i._arrivalTime == 6000){
            sixK = _pageFault/6000;
        }
        if(i._arrivalTime == 8000){
            eightK = _pageFault/8000;
        }
        if(i._arrivalTime == 10000){
            tenK = _pageFault/10000;
        }
    }
    totalPF = _pageFault;
}
//Optimal The process that will be used last "has longest time till next arrival" is replaced.
void OPTalg(std::vector<Proc> &list, int _frameSize, float &twoK, float &fourK, float &sixK, float &eightK, float &tenK, float &totalPF){
    std::vector<Proc> _memory;
    int time = 0;
    float _pageFault = 0;
    int _kickOut = 0;
    bool _inmemory;
    for(auto i : list) {
        _inmemory = inMemory(i, _memory);
        if(!_inmemory){
            _pageFault++;//inc fault
            if(_memory.size() < _frameSize){
                for(int k = 0; k < _memory.size(); ++k){
                    _memory[k]._pCount--;
                }
                //find next occuring process
                time = findNextOccur(i._pValue,i._arrivalTime, list);
                if(time == 0){
                    i._pCount = list.size();
                    _memory.push_back(i);
                }else{
                    i._pCount = (time-i._arrivalTime);
                    _memory.push_back(i);
                }
            }else{
                for(int k = 0; k < _memory.size(); ++k){
                    _memory[k]._pCount--;
                }
                //replace when found
                _kickOut = longestVect(_memory);
                _memory[_kickOut]._pValue = i._pValue;
                _memory[_kickOut]._arrivalTime = i._arrivalTime;
                time = findNextOccur(i._pValue,i._arrivalTime, list);
                if(time == 0){
                    _memory[_kickOut]._pCount = list.size();// - i._arrivalTime;
                } else
                    _memory[_kickOut]._pCount = (time-i._arrivalTime);
            }
        }else{
            int pos = 0;
            int count = 0;
            for(int k = 0; k < _memory.size(); ++k){
                _memory[k]._pCount--;
            }
            for(auto j : _memory){
                if(j._pValue == i._pValue){
                    pos = count;
                }
                count++;
            }
            time = findNextOccur(i._pValue,i._arrivalTime, list);
            if(time == 0){
                _memory[pos]._pCount = list.size();
            } else
                _memory[pos]._pCount = (time - i._arrivalTime);
        }
        //calculate results at different faults
        if(i._arrivalTime == 2000) {
            twoK = _pageFault/2000;
        }
        if(i._arrivalTime == 4000){
            fourK = _pageFault/4000;
        }
        if(i._arrivalTime == 6000){
            sixK = _pageFault/6000;
        }
        if(i._arrivalTime == 8000){
            eightK = _pageFault/8000;
        }
        if(i._arrivalTime == 10000){
            tenK = _pageFault/10000;
        }
    }
    totalPF = _pageFault;
}
//loop the memory and compare the process number to see if the process is already in memory or not.
bool inMemory(Proc &List,std::vector<Proc> &memory ){
    for(auto i : memory)
        //if the values are equal ex: 123 == 123 process is already in memory no page fault.
        if(i._pValue == List._pValue){
            return true;
        }
    return false;
}
//find the process in memory that has the first arrival.
int firstArrival(std::vector<Proc> &memory){
    int first = 10000;
    int pos = 0;
    int vpos = 0;
    //pos holds the position in the
    for(auto i : memory){
        pos++;
        if(i._arrivalTime < first){
            first = i._arrivalTime;
            vpos = pos;
        }
    }
    return vpos;
}
//used in LFU to find the process with the lowest frequency if there is a tie then broken by fifo
int lowestCount(std::vector<Proc> &memory){
    int lowest = 10000;
    int first = 10000;
    int R = 0;
    int count = 0;
    int vpos = 0;
    //create a vector to hold the processes with the lowest count
    std::vector<Proc> low;
    for(auto i : memory){
        if(i._pCount < lowest){
            lowest = i._pCount;
        }
    }
    //std::cout << lowest << '\n';
    //push processes with count == to lowest into the low vector
    for(auto k : memory){
        if(k._pCount == lowest){
            low.push_back(k);
        }
    }
    //ties for count to break the tie by FIFO.
    for(auto j : low){
        if(j._arrivalTime < first){
            first = j._arrivalTime;
            R = j._pValue;
        }
    }
    //get the position in the memory that needs to be replaced.
    for(auto l : memory){
        if(l._pValue == R){
            vpos = count;
        }
        count++;
    }

    return vpos;
}
//used in LRU to find the process with longest time unused ties broken with fifo
int highestCount(std::vector<Proc> &memory){
    int highest = 1;
    int first = 10000;
    int R = 0;
    int count = 0;
    int vpos = 0;
    std::vector<Proc> high;
    for(auto i : memory){
        if(i._pCount > highest){
            highest = i._pCount;
        }
    }
    for(auto k : memory){
        if(k._pCount == highest){
            high.push_back(k);
        }
    }

    for(auto j : high){
        if(j._arrivalTime < first){
            first = j._arrivalTime;
            R = j._pValue;
        }
    }

    for(auto l : memory){
        if(l._pValue == R){
            vpos = count;
        }
        count++;
    }

    return vpos;
}
//Used to find when the next time the process will occur
int findNextOccur(int value, int arrival, std::vector<Proc> &memory){
    auto memsize = int(memory.size());
    for(int i = arrival; i < memory.size(); ++i){
        if(value == memory[i]._pValue){
            int ar = memory[i]._arrivalTime;
            return memory[i]._arrivalTime;
        }
    }
    //std::cout << memsize;
    return 0;
}
//used to find the process in memory with the longest time till procs again to be replaced.
int longestVect(std::vector<Proc> &memory){
    int longest = 0;
    int first = 10000;
    int R = 0;
    int count = 0;
    int vpos = 0;
    std::vector<Proc> Long;
    for(auto i : memory){
        if(i._pCount > longest){
            longest = i._pCount;
        }
    }
    for(auto k : memory){
        if(k._pCount == longest){
            Long.push_back(k);
        }
    }
    for(auto j : Long){
        if(j._arrivalTime < first){
            first = j._arrivalTime;
            R = j._pValue;
        }
    }
    for(auto l : memory){
        if(l._pValue == R){
            vpos = count;
        }
        count++;
    }

    return vpos;
}

#endif //MEMSIM_MEMSIM_H
