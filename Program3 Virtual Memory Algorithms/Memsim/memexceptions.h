//
// Created by Chase Brown on 11/19/2017.
//

#ifndef MEMSIM_MEMEXCEPTIONS_H
#define MEMSIM_MEMEXCEPTIONS_H
#include <exception>
#include <iostream>

//the value of the no of pages is not of 128 256 512 or 1024
class NoOfPageexception : public std::exception{
public:
    explicit NoOfPageexception(std::string msg) : m_message(msg){}
    const char * what() const throw(){
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//error not enough values passed as parameters
class NotEnoughParams : public std::exception{
public:
    explicit NotEnoughParams(std::string msg) : m_message(msg){}
    const char * what() const throw(){
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//file fails to load
class LoadException : public std::exception{
public:
    explicit LoadException(std::string msg) : m_message(msg){}
    const char * what() const throw(){
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//test input number
class NumberFail : public std::exception{
public:
    explicit NumberFail(std::string msg) : m_message(msg){}
    const char * what() const throw(){
        return m_message.c_str();
    }
private:
    std::string m_message;
};
#endif //MEMSIM_MEMEXCEPTIONS_H
