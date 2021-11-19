/**
 * @file RandomNumberGenerator.cpp
 * @author Eren Albayrak & Marius Wichtner (Modified and Updated by Christopher Tran)
 * @brief Initial source code taken from the github repository below.
 *        https://github.com/erenalbayrak/Odd-Even-Sort-mit-MPI
 *
 *        The code has been modified and updated for CSCE-435 by Christopher Tran and Rohit Barichello
 *        Modifications include adding timing and evaluation metrics for determining 
 *        the sorting algorithm's performance. Additionally, comments were added for
 *        clarification, learning, and understanding purposes.
 */

#include <cstring>
#include <iostream>
#include <string>

enum listType { RANDOM,
                SORTED,
                REVERSED };

std::string create_file_name(char *strCountNumbersToGenerate, int type) {
    std::string file_name = strCountNumbersToGenerate;

    switch (type) {
        case RANDOM:
            file_name += "_random_";
            break;
        case SORTED:
            file_name += "_sorted_";
            break;
        case REVERSED:
            file_name += "_reversed_";
            break;
    }

    file_name += "numbers.bin";

    return file_name;
}

/** 
 * This program generates a binary file full of random numbers, to be read by the parallel
 * sorting algorithms used in this project
 * 
 * Compile: g++ RandomNumberGenerator.cpp -o RandomNumberGenerator
 * Run: ./RandomNumberGenerator <SIZE> <TYPE>
 * 
 * Arguments
 * SIZE: Size of random array
 * TYPE: Type of array. Choices are random, sorted and reversed. Pass 1, 2, or 3 respectively for each option
 */
int main(int argc, char *argv[]) {
    // Store file name created from arguments passed in
    std::string file_name = create_file_name(argv[1], atoi(argv[2]));

    // Open file with corresponding file name
    FILE *resultFile = fopen(file_name.c_str(), "wb");
    if (resultFile == nullptr) {
        std::cout << "Error. Can't open the result file." << std::endl;
        return EXIT_FAILURE;
    }

    // Generate specified number of random numbers
    srand(time(NULL));
    unsigned int numbers_to_generate = atoi(argv[1]);
    for (unsigned i = 0; i < numbers_to_generate; i++) {
        int number;

        switch (atoi(argv[2])) {
            case RANDOM:
                number = rand();
                break;
            case SORTED:
                number = i;
                break;
            case REVERSED:
                number = numbers_to_generate - i;
                break;
        }

        fwrite(&number, sizeof(number), 1, resultFile);

        // Output to console as necessary
        // if (argc > 2 && strcmp(argv[2], "y") == 0) {
        //     std::cout << number << " ";
        // }
    }

    std::cout << std::endl << "Result file created: " << file_name << std::endl;

    // Close the file with corresponding file name
    fclose(resultFile);
    return EXIT_SUCCESS;
}
