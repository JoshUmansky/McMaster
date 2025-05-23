/*
Comp Eng 3DY4 (Computer Systems Integration Project)

Department of Electrical and Computer Engineering
McMaster University
Ontario, Canada
*/

#ifndef DY4_IOFUNC_H
#define DY4_IOFUNC_H

// add headers as needed
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <complex>

// declaration of a function prototypes
void printRealVector(const std::vector<float> &);

void printComplexVector(const std::vector<std::complex<float>> &);

void readBinData(const std::string, std::vector<float> &);

void writeBinData(const std::string, const std::vector<float> &);

void readFileData(const std::vector<float> I, const std::vector<float> Q);

void readStdinBlockData(unsigned int num_samples, unsigned int block_id, std::vector<float> &block_data);

#endif // DY4_IOFUNC_H
