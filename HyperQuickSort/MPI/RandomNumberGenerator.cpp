/**
 * @file RandomNumberGenerator.cpp
 * @author Eren Albayrak & Marius Wichtner (Modified and Updated by Christopher Tran)
 * @brief Initial source code taken from the github repository below.
 *        https://github.com/erenalbayrak/Odd-Even-Sort-mit-MPI
 *
 *        The code has been modified and updated for CSCE-435 by Christopher Tran
 *        Modifications include adding timing and evaluation metrics for determining 
 *        the sorting algorithm's performance. Additionally, comments were added for
 *        clarification, learning, and understanding purposes.
 */

#include <iostream>
#include <string>
#include <cstring>

/**
 * @brief Create a file name object
 * 
 * @param strCountNumbersToGenerate 
 * @return std::string 
 */
std::string create_file_name(char *strCountNumbersToGenerate)
{
    std::string file_name = strCountNumbersToGenerate;
    file_name += "numbers";
    file_name += ".bin";

    return file_name;
}

/** 
 * This program generates random numbers.
 * The result will save into a file named "<countNumbersToGenerate>numbers.bin"
 * Example: "1000numbers.bin"
 * The file will created in the same directory, where this program will executed."
 *
 * Compile: g++ RandomNumberGenerator.cpp -o RandomNumberGenerator
 * Call with output on console:     ./RandomNumbers 1600 y
 * Call without output on console:  ./RandomNumbers 1600
 */
int main(int argCount, char *argValues[])
{
    // Store file name created from arguments passed in
    std::string file_name = create_file_name(argValues[1]);

    // Open file with corresponding file name
    FILE *resultFile = fopen(file_name.c_str(), "wb");
    if (resultFile == nullptr)
    {
        std::cout << "Error. Can't open the result file." << std::endl;
        return EXIT_FAILURE;
    }

    // Generate specified number of random numbers
    srand(time(NULL));
    unsigned int numbers_to_generate = atoi(argValues[1]);
    for (unsigned i = 0; i < numbers_to_generate; i++)
    {
        int randNumber = rand();
        fwrite(&randNumber, sizeof(randNumber), 1, resultFile);

        // Output to console as necessary
        if (argCount > 2 && strcmp(argValues[2], "y") == 0)
        {
            std::cout << randNumber << " ";
        }
    }

    std::cout << std::endl
              << "Result file created: " << file_name << std::endl;

    // Close the file with corresponding file name
    fclose(resultFile);
    return EXIT_SUCCESS;
}