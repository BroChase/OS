//
// Created by Chase Brown on 9/23/2017.
//

#ifndef BROOSLAB1_MATRIXEXCEPTIONS_H
#define BROOSLAB1_MATRIXEXCEPTIONS_H
#include <exception>
#include <iostream>
//load exception for file failure
class LoadException : public std::exception
{
public:
    explicit LoadException(std::string msg) : m_message(msg) { }
    const char * what () const throw ()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//Matrix dimes error Mxn nxP if n != n error
class matrixAdims : public std::exception
{
public:
    explicit matrixAdims(std::string msg) : m_message(msg) { }
    const char * what () const throw ()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//error reading in file data.  if not number throw error
class notNumber : public std::exception
{
public:
    explicit notNumber(std::string msg) : m_message(msg) { }
    const char * what () const throw ()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//out of range of long long int
class outofrange : public std::exception
{
public:
    explicit outofrange(std::string msg) : m_message(msg) { }
    const char * what () const throw ()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};
//can not create new matrix for product must be MxP
class matrixMP : public std::exception
{
public:
    explicit matrixMP(std::string msg) : m_message(msg) { }
    const char * what () const throw ()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};
#endif //BROOSLAB1_MATRIXEXCEPTIONS_H
