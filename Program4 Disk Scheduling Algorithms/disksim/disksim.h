//
// Created by Chase Brown on 11/20/2017.
//

#ifndef DISKSIM_DISKSIM_H
#define DISKSIM_DISKSIM_H
#include <iostream>
#include <fstream>
#include <exception>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <vector>
#include <queue>
#include <stack>
#include "diskexceptions.h"

struct diskPosition{
    int _location = 0;
    bool _que = true;
};
struct sortDisk{
    inline bool operator()(const diskPosition& first, const diskPosition& second){
        return (first._location < second._location);
    }
};
struct sortDiskreverse{
    inline bool operator()(const diskPosition& first, const diskPosition& second){
        return (first._location > second._location);
    }
};

void evaluate(int, int, const std::string &, const std::string &);
void readFile(std::vector<diskPosition> &, const std::string &);
void FCFSalg(std::vector<diskPosition> &, int, std::ofstream &);
void SSTFalg(std::vector<diskPosition> &, int, std::ofstream &);
void SCANalg(std::vector<diskPosition> &, int, int, std::ofstream &);
void CLOOKalg(std::vector<diskPosition> &, int, int, std::ofstream &);
int distance(std::vector<diskPosition> &, int);

void evaluate(int _position, int _direction, const std::string &_inFile, const std::string &_outFile){
    //dicrection of movement.
    std::string direction;
    if(_direction == 0){
        direction = "Down";
    }else{
        direction = "Up";
    }

    std::ofstream outFile(_outFile);
    try{
        if(!outFile){
            throw LoadException(std::string("OutPut File Failed to open"));
        }
    }catch(const LoadException& e){
        std::cout<< e.what() << '\n';
        exit(0);
    }
    //create a vector to hold the list of 10,000 processes
    std::vector<diskPosition> _disk;
    _disk.emplace_back();
    //readfile will read the contents of the file and place them into the process list vector.
    readFile(_disk, _inFile);
    //remove the initial value pushed into the vector.
    _disk.erase(_disk.begin());

    outFile << "===============================================================" << '\n';
    outFile << "[FCFS] Disk Scheduling Algorithm Simulation" << '\n';
    outFile << "Current disk head position " << _position << " and is moving " << direction << '\n';
    outFile << "===============================================================" << '\n';
    FCFSalg(_disk, _position, outFile);
    outFile << '\n';
    outFile << "===============================================================" << '\n';
    outFile << "[SSTF] Disk Scheduling Algorithm Simulation" << '\n';
    outFile << "Current disk head position " << _position << " and is moving " << direction << '\n';
    outFile << "===============================================================" << '\n';
    SSTFalg(_disk, _position, outFile);
    outFile << '\n';
    outFile << "===============================================================" << '\n';
    outFile << "[SCAN] Disk Scheduling Algorithm Simulation" << '\n';
    outFile << "Current disk head position " << _position << " and is moving " << direction << '\n';
    outFile << "===============================================================" << '\n';
    SCANalg(_disk, _position, _direction, outFile);
    outFile << '\n';
    outFile << "===============================================================" << '\n';
    outFile << "[C-LOOK] Disk Scheduling Algorithm Simulation" << '\n';
    outFile << "Current disk head position " << _position << " and is moving " << direction << '\n';
    outFile << "===============================================================" << '\n';
    CLOOKalg(_disk, _position, _direction, outFile);
    outFile.close();
}
void readFile(std::vector<diskPosition> &disk, const std::string &file){
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
    //extract the values and push into the "disk".
    while(!_infile.eof()){
        diskPosition number = diskPosition();
        std::string line;
        std::getline(_infile, line, ',');
        if(!line.empty()){
            try {
                number._location = std::stoi(line);
            }catch(const std::exception& e){
                std::cout << e.what() << " Failed, bad input" << '\n';
                exit(0);
            }
            count++;
            disk.push_back(number);
        }
    }
}
void FCFSalg(std::vector<diskPosition> &disk, int _position, std::ofstream &out){
    int current = _position;
    int count = 0;
    int cylinder = 0;
    for(auto i : disk){
        out << "[" << current<<"->" << i._location << "] ";
        cylinder += abs(i._location - current);
        current = i._location;
        count++;
        if(count == 6){
            out << '\n';
            count = 0;
        }
    }
    out << '\n';
    out << "Total number of cylinder movements: " << cylinder << '\n';
}
void SSTFalg(std::vector<diskPosition> &disk, int _position, std::ofstream &out){
    int current = _position;
    int count = 0;
    int cylinder = 0;
    int vpos = 0;
    for(auto i : disk){
        out << "[" << current << "->";
        vpos = distance(disk, current);
        out << disk[vpos]._location << "] ";
        cylinder += abs(current - disk[vpos]._location);
        disk[vpos]._que = false;
        current = abs(disk[vpos]._location);
        count++;
        if(count == 6){
            out << '\n';
            count = 0;
        }
    }
    out << '\n';
    out << "Total number of cylinder movements: " << cylinder << '\n';
}
void SCANalg(std::vector<diskPosition> &disk, int _position, int _direction, std::ofstream &out){
    std::vector<diskPosition> _sorted;
    std::stack<diskPosition> _half;
    std::queue<diskPosition> _upper;
    diskPosition p;
    int cylinder = 0;
    int current = _position;
    int R = 0;

    for(auto i: disk){
        _sorted.push_back(i);
    }
    //todo sort objects
    //if direction is "up" increasing
    if(_direction == 1){
        //sort the list lowest to highest.
        std::sort(_sorted.begin(),_sorted.end(), sortDisk());
        int count = 0;
        //if the process is less then _position then push them into a stack and remove them from vector
        for(auto k : _sorted){
            if(k._location < _position){
                _half.push(k);
                count++;
            }
        }
        //remove the elements from beginning of list that were pushed to back
        while(count != 0){
            _sorted.erase(_sorted.begin());
            count--;
        }
        //print out the values => starting point.
        for(auto n : _sorted){
            out << "[" << current << "->"<<n._location<<"] ";
            cylinder += abs(current - n._location);
            current = n._location;
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
        }
        //if there are processes below the starting point. else done
        if(!_half.empty()){
            out << "[" << current << "->" << 199 << "] ";
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
            cylinder += abs(current - 199);
            if(!_half.empty()){
                p = _half.top();
                out << "[" << 199 << "->" << p._location << "] ";
                cylinder += abs(199- p._location);
                current = p._location;
                _half.pop();
                R++;
                if(R == 6){
                    out << '\n';
                    R = 0;
                }
            }
            while(!_half.empty()){
                p = _half.top();
                out << "[" << current << "->" << p._location << "] ";
                cylinder += abs(current-p._location);
                current = p._location;
                _half.pop();
                R++;
                if(R == 6){
                    out << '\n';
                    R = 0;
                }
            }
        }
    }else{
        //sort the list lowest to highest.
        std::sort(_sorted.begin(),_sorted.end(), sortDisk());
        int count = 0;
        //if the process is less then _position then push them into a stack and remove them from vector
        for(auto k : _sorted){
            if(k._location > _position){
                _upper.push(k);
                count++;
            }
        }
        //vector _sorted holds the lower half _upper holds the upper half.
        while(count !=0){
            _sorted.pop_back();
            count--;
        }
        //sort the list highest to lowest.
        std::sort(_sorted.begin(),_sorted.end(), sortDiskreverse());
        //print out the values => starting point.
        for(auto n : _sorted){
            out << "[" << current << "->"<<n._location<<"] ";
            cylinder += abs(current - n._location);
            current = n._location;
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
        }
        if(!_upper.empty()){
            out << "[" << current << "->" << 0 << "] ";
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
            cylinder += abs(0-current);
            if(!_upper.empty()){
                p = _upper.front();
                out << "[" << 0 << "->" << p._location << "] ";
                cylinder += abs(0 - p._location);
                current = p._location;
                _upper.pop();
                R++;
                if(R == 6){
                    out << '\n';
                    R = 0;
                }
            }
            while(!_upper.empty()){
                p = _upper.front();
                out << "[" << current << "->" << p._location << "] ";
                cylinder += abs(current-p._location);
                current = p._location;
                _upper.pop();
                R++;
                if(R == 6){
                    out << '\n';
                    R = 0;
                }
            }
        }
    }
    out << '\n';
    out << "Total number of cylinder movements: " << cylinder << '\n';
}
void CLOOKalg(std::vector<diskPosition> &disk, int _position, int _direction, std::ofstream &out){
    std::vector<diskPosition> _sorted;
    std::stack<diskPosition> _half;
    std::queue<diskPosition> _upper;
    diskPosition p;
    int cylinder = 0;
    int current = _position;
    int R = 0;

    for(auto i: disk){
        _sorted.push_back(i);
    }
    //todo sort objects
    //if direction is "up" increasing
    if(_direction == 1){
        //sort the list lowest to highest.
        std::sort(_sorted.begin(),_sorted.end(), sortDisk());
        int count = 0;
        //if the process is less then _position then push them into a stack and remove them from vector
        for(auto k : _sorted){
            if(k._location < _position){
                _upper.push(k);
                count++;
            }
        }
        //remove the elements from beginning of list that were pushed to back
        while(count != 0){
            _sorted.erase(_sorted.begin());
            count--;
        }
        while(!_upper.empty()){
            p = _upper.front();
            _sorted.push_back(p);
            _upper.pop();
        }
        //print out the values => starting point.
        for(auto n : _sorted){
            out << "[" << current << "->"<<n._location<<"] ";
            cylinder += abs(current - n._location);
            current = n._location;
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
        }
    }else{
        //sort the list lowest to highest.
        std::sort(_sorted.begin(),_sorted.end(), sortDisk());
        int count = 0;
        //if the process is less then _position then push them into a stack and remove them from vector
        for(auto k : _sorted){
            if(k._location > _position){
                _half.push(k);
                count++;
            }
        }
        //vector _sorted holds the lower half _upper holds the upper half.
        while(count !=0){
            _sorted.pop_back();
            count--;
        }
        //sort the list highest to lowest.
        std::sort(_sorted.begin(),_sorted.end(), sortDiskreverse());
        //print out the values => starting point.
        for(auto n : _sorted){
            out << "[" << current << "->"<<n._location<<"] ";
            cylinder += abs(current - n._location);
            current = n._location;
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
        }
        while(!_half.empty()){
            p = _half.top();
            out << "[" << current << "->" << p._location << "] ";
            cylinder += abs(current - p._location);
            current = p._location;
            _half.pop();
            R++;
            if(R == 6){
                out << '\n';
                R = 0;
            }
        }
    }
    out << '\n';
    out << "Total number of cylinder movements: " << cylinder << '\n';
}
int distance(std::vector<diskPosition> &disk, int _cPosition){
    int distance = 200;
    int count = 0;
    int vpos = 0;
    for(auto i : disk){
        if(i._que && abs(i._location - _cPosition) < distance){
            distance = abs(i._location-_cPosition);
            vpos = count;
            //count++;
        }
        count++;
    }
    return vpos;
}



#endif //DISKSIM_DISKSIM_H
