//
// Created by Chase Brown on 9/22/2017.
//

#ifndef BROOSLAB1_MATRIXFUNCTIONS_H
#define BROOSLAB1_MATRIXFUNCTIONS_H

#include <iostream>
#include <chrono>
#include <fstream>
#include <exception>
#include <random>
#include <limits>
#include <cmath>
#include <pthread.h>
#include <string>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include "matrixexceptions.h"


typedef long long int bignumber;

//struct to be passed into the void* function
struct matrixStruct {
    bignumber **matrixA;
    bignumber **matrixB;
    bignumber **product;
    int row;
    int col;
    int row2;
    int startthread;
    int finishthread;
};


void *matrixthreading(void *matrixdata) {
    struct matrixStruct *matrix_data;
    matrix_data = (struct matrixStruct *) matrixdata;

    for (int row = matrix_data->startthread; row < matrix_data->finishthread; row++) {
        //std::cout << matrix_data->row << '\n';
        for (int col = 0; col < matrix_data->col; col++) {
            //std::cout << matrix_data->col << '\n';
            long long int sumofproducts = 0;
            for (int inner = 0; inner < matrix_data->row2; inner++) {

                sumofproducts += matrix_data->matrixA[row][inner] * matrix_data->matrixB[inner][col];
                //std::cout<<matrix_data->product[row][col]<< " ";
            }
            matrix_data->product[row][col] = sumofproducts;
            //std::cout << '\n';
        }
    }
    delete matrix_data;
    pthread_exit(nullptr);
}

template<typename T>
bool isNumber(T x){
    std::string s;
    std::stringstream ss;
    ss << x;
    ss >>s;
    if(s.empty() || std::isspace(s[0]) || std::isalpha(s[0])) return false ;
    char * p ;
    strtod(s.c_str(), &p) ;
    return (*p == 0) ;
}

void createNewMatrix() {
    std::string checkvar;
    bignumber number;
    std::string filename;
    int rowsA, colsA, rowsB, colsB, numberOfThreads, choice;


    std::cout << "Enter filename that holds matrix A and matrix B" << '\n' << "ex: matrixfile.txt" << '\n';
    std::cin >> filename;
    std::cin.ignore();
    std::ifstream infile(filename);
    std::cout << "Number of Threads to use for operation (1-5000)?\n";
    std::cin >> numberOfThreads;
    std::cin.ignore();
    if(numberOfThreads < 1 || numberOfThreads > 5000){
        std::cout<< "Invalid number\n";
        createNewMatrix();
    }
    try{
    if(!infile){
        throw LoadException(std::string("File Error, Exiting Program"));
    }
    }catch (const LoadException& e) {
        std::cout<< e.what() << '\n';
        exit(0);
    }

    while (!infile.eof()) {
        //read in matrixA dim
        infile >> rowsA;
        infile >> colsA;
        //check for the dims of matrix A to be at least 1x1 and no greater than 10000x10000 matrix.
        //todo check the rowsA and colsA to see if they are numbers first
        try{
            if(rowsA < 1 || rowsA > 10000 || colsA < 1 || colsA > 10000){
                throw matrixAdims(std::string("MatrixA dim error, Exiting Program"));
            }
        }catch(const matrixAdims& e){
            std::cout<< e.what() << '\n';
            exit(0);
        }
        try{
            if(isdigit(rowsA) || isdigit(colsA)){
                throw notNumber(std::string("Variable in matrix Dims is not an int"));
            }
        }catch(notNumber& e){
            std::cout << e.what() << '\n';
            exit(0);
        }
        //create dynamic 2d array for matrixA[r][c]
        auto **matrixA = new bignumber *[rowsA];
        for (int i = 0; i < rowsA; ++i) {
            matrixA[i] = new bignumber[colsA];
        }
        try{
        for (int k = 0; k < rowsA; ++k) {
            for (int l = 0; l < colsA; ++l) {
                infile >> checkvar;
                //check input variables before placing in matrix
                try{
                    number = std::stoll(checkvar);
                }catch(std::out_of_range& e){
                    std::cout<<"Can not fit into Long Long GoodBye";
                    exit(0);
                }catch(std::invalid_argument& e){
                    std::cout<<"Variable is not a number";
                    exit(0);
                }
                matrixA[k][l] = number;
            }
        }
        }catch(const outofrange& e){
            std::cout << e.what() << '\n';
            exit(0);
        }catch(const notNumber& e){
            std::cout << e.what() << '\n';
            exit(0);
        }




        //read in matrixB dim
        infile >> rowsB;
        infile >> colsB;
        //check matrixB dims
        try{
            if(rowsB < 1 || rowsB > 10000 || colsB < 1 || colsB > 10000){
                throw matrixAdims(std::string("MatrixB dim error, Exiting Program"));
            }
        }catch(const matrixAdims& e){
            std::cout<< e.what() << '\n';
            exit(0);
        }
        try{
            if(isdigit(rowsB) || isdigit(colsB)){
                throw notNumber(std::string("Variable in matrix Dims is not an int"));
            }
        }catch(notNumber& e){
            std::cout << e.what() << '\n';
            exit(0);
        }
        //create dynamic 2d array for matrixB[r][c]
        auto **matrixB = new bignumber *[rowsB];
        for (int i = 0; i < rowsB; ++i) {
            matrixB[i] = new bignumber[colsB];
        }
        try {
            for (int k = 0; k < rowsB; ++k) {
                for (int l = 0; l < colsB; ++l) {
                    infile >> checkvar;
                    try{
                        number = std::stoll(checkvar);
                    }catch(std::out_of_range& e){
                        std::cout<<"Can not fit into Long Long GoodBye";
                        exit(0);
                    }catch(std::invalid_argument& e){
                        std::cout<<"Variable is not a number";
                        exit(0);
                    }
                    matrixB[k][l] = number;
                }
            }
        }catch(const notNumber& e){
            std::cout << e.what() << '\n';
            exit(0);
        }catch(const outofrange& e){
            std::cout << e.what() << '\n';
            exit(0);
        }
        //matrix A rows must == matrix B cols for matrix multiplication.
        try {
            if (rowsA != colsB) {
                throw matrixMP(std::string("Unable to do matrix multiplications.  Rows of A do not == Cols of B"));
            }
        }catch(matrixMP& e){
            std::cout << e.what() << '\n';
            exit(0);
        }
//*******************************************************************
        //print matrix A
        std::cout << "Matrix A:" << '\n';
        for (int k = 0; k < rowsA; ++k) {
            for (int l = 0; l < colsA; ++l) {
                std::cout << matrixA[k][l] << ' ';
            }
            std::cout << '\n';
        }
        //print out matrixB
        std::cout << "Matrix B:" << '\n';
        for (int k = 0; k < rowsB; ++k) {
            for (int l = 0; l < colsB; ++l) {
                std::cout << matrixB[k][l] << ' ';
            }
            std::cout << '\n';
        }
        //create a matrix to hold the end result of matrix multiplication.
        auto **product = new bignumber *[rowsA];
        for (int i = 0; i < rowsA; ++i) {
            product[i] = new bignumber[colsB];
        }
        pthread_t threads[numberOfThreads];
        //void status to be used inside of the ptrhead_join
        void *status;
        //matrixStruct1.startthread = 0;
        int starter = 0;
        float r = rowsA;
        float n = numberOfThreads;
        float cel = ceil(r/n);
        //float cel1 = cel+1;
        int finisher = cel;
        //std::cout<<finisher<< '\n';
        //matrixStruct1.finishthread = starter;
        int ret;

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for (int i = 0; i < numberOfThreads; ++i) {
            auto *p = new matrixStruct;
            p->matrixA = matrixA;
            p->matrixB = matrixB;
            p->product = product;
            p->row = rowsA;
            p->col = colsB;
            p->row2 = rowsB;
            p->startthread = starter;
            p->finishthread = finisher;
            ret = pthread_create(&threads[i], &attr, matrixthreading, (void *) p);

            if(ret){
                std::cout<<"Error creating thread\n";
            }
            //pthread_t thId = pthread_self();
            std::cout << "Creating worker thread for "<< threads[i]<< " row " << starter << " to " << finisher << '\n';

            starter += cel;
            finisher += cel;
                if(finisher >= rowsA){
                    finisher = rowsA;
                }
        }
        pthread_attr_destroy(&attr);
        for(int i = 0; i < numberOfThreads; ++i){
            ret = pthread_join(threads[i], &status);
            if(ret){
                std::cout << "Join Failed \n";
            }
        }
        end = std::chrono::system_clock::now();
        std::cout << "\n";
        std::cout << "Matrix C:" << "\n";
        for (int i = 0; i < rowsA; ++i) {
            for (int k = 0; k < colsB; ++k) {
                std::cout << product[i][k] << " ";
            }
            std::cout << '\n';
        }
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout<<"Total execution time using " << numberOfThreads << " threads is "<< elapsed_seconds.count() <<" s \n";



        //clean up matrices
        for (int i = 0; i < rowsA; ++i) {
            delete[] matrixA[i];
        }
        delete[] matrixA;

        for (int i = 0; i < rowsB; ++i) {
            delete[] matrixB[i];
        }
        delete[] matrixB;

        for (int i = 0; i < rowsA; ++i) {
            delete[] product[i];
        }
        delete[] product;
    }

    std::cout << "\n Would you like to read in another set of Matrices? \n 1. Yes \n 2. Exit \n ";
    std::cin >> choice;
    std::cin.ignore();
    if(choice == 1){
        createNewMatrix();
    } else
        exit(0);
}

//main start menu
void mainmenu() {
    int choice;
    std::cout << "Welcome to OS Lab 1 Matrix Multiplication" << '\n';
    std::cout << "1. Begin" << '\n';
    std::cout << "2. Exit" << '\n';
    std::cin >> choice;
    std::cin.ignore();
    try {
        if (choice == 1) {
            createNewMatrix();
        } else if(choice == 2) {
            std::cout << "Goodbye";
        } else
            throw 1;

    }catch(int){std::cout << "Invalid Option Goodbye";}
}
//used to create and test different matrices
void createMatrix(int row, int col, std::string filename) {
    std::ofstream outfile(filename);
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<int> distr;
    outfile << row << ' ' << col << '\n';
    for (int i = 0; i < row; ++i) {
        for (int k = 0; k < col; ++k) {
            int n = distr(eng)%10;
            outfile << n;
            if (k != (col - 1)) {
                outfile << " ";
            }
        }
        outfile << '\n';
    }
    outfile << row << ' ' << col << '\n';
    for (int i = 0; i < row; ++i) {
        for (int k = 0; k < col; ++k) {
            int n = distr(eng)%10;
            outfile << n;
            if (k != (col - 1)) {
                outfile << " ";
            }
        }
        if(i != row-1){
        outfile << '\n';
        }
    }
}
#endif //BROOSLAB1_MATRIXFUNCTIONS_H
