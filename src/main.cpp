///////////////////////////////////////////////////////////////////////////////
/// @file main.cpp
/// @author Olivier Cotte
/// @date February 2014
///////////////////////////////////////////////////////////////////////////////

/*
 *********************************************************************************************************
 *                                              INCLUDES
 *********************************************************************************************************
 */
#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <iostream>

#include "argvparser/argvparser.h"
#include "Sort.hpp"

using namespace CommandLineProcessing;

/*
 *********************************************************************************************************
 *                                              DEFINITION
 *********************************************************************************************************
 */

typedef long long timestamp_t;

/*
 *********************************************************************************************************
 *                                         FUNCTION PROTOTYPES
 *********************************************************************************************************
 */

timestamp_t get_timestamp();

template <typename T>
void print(T a[], size_t size);

template <typename T>
bool is_sorted(T a[], size_t size);

template <typename T>
void extract_sample_from_file(const std::string &in_strFilePath, std::vector<T> &inout_vecSample);

/*
 *********************************************************************************************************
 *                                                  MAIN
 *********************************************************************************************************
 */

/// Main function of the application which is actually its entry point
///
/// @param argv Parameters passed to the application by the command line
/// Each executable must have the following parameters:
///		-f path to the textfile containing the sequence to be sorted,
///		-p to print the sorted vector.
/// If executed without the -p parameter, the program will only display its execution time.
int main(int argc, char *argv[])
{
	// Get command line information
	ArgvParser cmd;

	cmd.defineOption("f", "", ArgvParser::OptionRequiresValue);
	cmd.defineOption("p");

	// Parse and handle return codes (display help etc...)
	int result = cmd.parse(argc, argv);

	if (result != ArgvParser::NoParserError) {
		std::cout << cmd.parseErrorDescription(result);
		exit(1);
	}

	// Process the information obtained on the command line
	if (cmd.foundOption("f")) {
		std::string filepath = cmd.optionValue("f");
		std::vector<int> samples;
		extract_sample_from_file(filepath, samples);
		if(samples.size()) {
			double t1 = get_timestamp();
			bubble(samples);
			//counting(samples);
			//quick(samples);
			//quickMed(samples);
			double t2 = get_timestamp();

			if(is_sorted(&samples[0], samples.size())) {
				if (cmd.foundOption("p")) {
					print(&samples[0], samples.size());
				}
				else {
					std::cout << "execution time: " << (t2-t1)/10e6 << " sec" << std::endl;
				}
			}
			else {
				std::cout << "unable to sort the input vector" << std::endl;
				return -1;
			}
		}
		else {
			std::cout << filepath << " is and invalid path" << std::endl;
			return -1;
		}
	}

	return 0;
}

/*
 *********************************************************************************************************
 *                                                  MAIN.CPP
 *********************************************************************************************************
 */

////////////////////////////////////////////////////////////////////////
///
/// @fn template < typename T > void print(T a[], size_t size)
///
/// Prints the sorted vector.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template <typename T>
void print(T a[], size_t size)
{
	for(size_t i = 0; i < size; ++i) {
		std::cout << a[i] << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <typename T> bool is_sorted(T a[], size_t size)
///
/// Checks if the vector is sorted.
///
/// @return True if the vector is sorted, false otherwise.
///
////////////////////////////////////////////////////////////////////////
template <typename T>
bool is_sorted(T a[], size_t size)
{
	const size_t l = size;
	for (size_t i = 1; i < l/2 + 1 ; ++i) {
		if (a[i - 1] > a[i] || a[l-i] < a[l-i-1]) {
			return false;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn timestamp_t get_timestamp()
///
/// Obtain the machine current time.
///
/// @return Current time in usec.
///
////////////////////////////////////////////////////////////////////////
timestamp_t get_timestamp()
{
	struct timeval now;
	gettimeofday (&now, NULL);
	return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn template <typename T> void extract_sample_from_file(const std::string &in_strFilePath, std::vector<T> &inout_vecSample)
///
/// Reads all integers from a file and saves them in a vector.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template <typename T>
void extract_sample_from_file(const std::string &in_strFilePath, std::vector<T> &inout_vecSample)
{
	int a;
	std::ifstream infile(in_strFilePath.c_str());
	if(!infile) return; // Fail to open
	while (infile >> a) inout_vecSample.push_back(a);
}
