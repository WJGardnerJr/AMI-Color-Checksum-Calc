#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <limits>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cstring>
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}
void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); // Wait for user to press Enter
}
void checksumCalculator(char& option, std::string& biosFileName) {
    do {
        if (option == '\0'){
            mainMenu:
            clearScreen();
            std::cout << "Please select an option:" << std::endl;
            std::cout << "==== MENU ====" << std::endl;
            std::cout << "1.) Read AMI Hi-Color BIOS file (and earlier)" << std::endl;
            std::cout << "2.) Read 32 KB VGA BIOS file" << std::endl;
            std::cout << "Type [Q/q] to quit" << std::endl << std::endl;
            std::cout << ">";
            std::cin >> option;
            if (std::tolower(option) == 'q') { break; }
            if (std::cin.fail()) {
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            }
        }
        switch(option) {
        case '1':
            returntoChoice1:
            try {
                if (biosFileName.empty()){
                    std::string exitString = "quit";
                    int bitSize = 0;
                    std::cout << "Please enter AMI Color BIOS (or older AMI 64 KB BIOS) file name: " << std::endl;
                    std::cout << "Type quit or Quit to return to main menu." << std::endl;
                    std::cout << "Type exit, q, or Q to exit the program." << std::endl;
                    std::cout << ">";
                    std::cin >> biosFileName;
                    std::transform(biosFileName.begin(), biosFileName.end(), biosFileName.begin(),
                    [](unsigned char c){ return std::tolower(c); });
                    int stringComp = (biosFileName).compare(exitString);
                    if (stringComp == 0) { goto mainMenu;}
                    if (biosFileName == "exit" || biosFileName == "q" || biosFileName == "Q") { exit(0); }
                }
                std::ifstream biosFile(biosFileName, std::ios::binary | std::ios::ate);
                if (!biosFile.is_open()) {
                    throw std::runtime_error("Failed to open bios file.");
                    pauseScreen();
                }
                std::streampos fileSize = biosFile.tellg();
                biosFile.seekg(0, std::ios::beg); // Seek back to the beginning to read the content

                std::vector<char> rawBiosData((std::istreambuf_iterator<char>(biosFile)),
                                            std::istreambuf_iterator<char>());

                biosFile.close(); // Close the file as its contents are now in rawBiosData

                const size_t blockSize = 16; // Calculate checksum based on blocks of 16 bytes
                size_t blockCount = fileSize / blockSize; //Calculate number of blocks
                //Print bios data
                std::cout << "\n===== " << biosFileName << " data ======" << std::endl;
                std::cout << "File size: " << fileSize / 1024 << " KB" << std::endl;
                std::cout << "Block count: " << blockCount << std::endl;

                int checksum = 0;
                for (size_t block = 0; block < blockCount; ++block) {
                    int block_checksum = 0;
                    //Sum each block's checksum
                    for (int byte_pair = 0; byte_pair < 8; ++byte_pair) {
                        if ((byte_pair * 2 + 1) < blockSize) {
                            unsigned char byte1 = rawBiosData[block * blockSize + byte_pair * 2];
                            unsigned char byte2 = rawBiosData[block * blockSize + byte_pair * 2 + 1];
                            std::stringstream ss;
                            ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte2)
                            << std::setw(2) << static_cast<int>(byte1);
                            std::string byte_pair_hex = ss.str();
                            int byte_pair_dec = std::stoi(byte_pair_hex, nullptr, 16);
                            block_checksum += byte_pair_dec; //Calculate total block checksum
                        }
                    }
                    checksum += block_checksum; //Add all block checksums to main checksum
                }
                checksum %= static_cast<int>(fileSize); // Calculate checksum
                if (checksum != 0) {
                    std::cout << "Checksum does not equal zero." << std::endl;
                    std::uint64_t twoByteAddition = static_cast<std::uint64_t>(fileSize) - static_cast<std::uint64_t>(checksum);
                    twoByteAddition = twoByteAddition & 0xFFFF;
                    
                    //Convert to little-endian
                    unsigned char lowByte = twoByteAddition & 0xFF;
                    unsigned char highByte = (twoByteAddition >> 8) & 0xFF;
                    std::ostringstream ss;
                    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
                    << static_cast<int>(lowByte) << " " << std::setw(2) << static_cast<int>(highByte);
                    std::string twoByteAddition_hex = ss.str();
                    std::cout << "Add the following two bytes somewhere in the bios file padding to get a checksum of zero: " << twoByteAddition_hex << std::endl;
                } else {
                    std::cout << "Checksum equals zero." << std::endl;
                }
                std::cout << "Checksum: " << checksum << std::endl;
                option = '\0';
                biosFileName.clear();
            }
            catch(const std::exception& e) { 
                std::cerr << "Exception caught: " << e.what() << std::endl;
                pauseScreen();
                option = '\0';
                biosFileName.clear();
                goto returntoChoice1;
            }
        break;
        case '2':
            returntoChoice2:
            try {
                if (biosFileName.empty()){
                    std::string exitString = "quit";
                    std::cout << "Please enter VGA bios file name: " << std::endl;
                    std::cout << "Type quit or Quit to return to main menu." << std::endl;
                    std::cout << "Type exit, q, or Q to exit the program." << std::endl;
                    std::cout << ">";
                    std::cin >> biosFileName;
                    std::cout << biosFileName;
                    std::transform(biosFileName.begin(), biosFileName.end(), biosFileName.begin(),
                    [](unsigned char c){ return std::tolower(c); });
                    int stringComp = (biosFileName).compare(exitString);
                    if (stringComp == 0) { goto mainMenu; }
                    if (biosFileName == "exit" || biosFileName == "q" || biosFileName == "Q") { exit(0); }
                }
                std::ifstream biosFile(biosFileName, std::ios::binary);
                if (!biosFile.is_open()) {
                    throw std::runtime_error("Failed to open bios file.");
                    pauseScreen();
                }
                std::vector<unsigned char> rawBiosData((std::istreambuf_iterator<char>(biosFile)),
                                                    std::istreambuf_iterator<char>());
                biosFile.close(); 
                // Calculate the 8-bit checksum by summing all bytes and taking modulus by 256
                unsigned long long sum = 0;
                for (unsigned char byte : rawBiosData) {
                    sum += byte;
                }
                int checksum = sum % 256;
                std::cout << "\n===== " << biosFileName << " data ======" << std::endl;
                std::cout << "File size: " << rawBiosData.size() / 1024 << " KB" << std::endl;
                std::cout << "Checksum: " << checksum << std::endl;
                if (checksum == 0) {
                    std::cout << "Checksum equals zero." << std::endl;
                } else {
                    std::cout << "Checksum does not equal zero." << std::endl;
                }
                option = '\0';
                biosFileName.clear();
            } catch(const std::exception& e) {
                std::cerr << "Exception caught: " << e.what() << std::endl;
                pauseScreen();
                option = '\0';
                biosFileName.clear();
                goto returntoChoice2;
            }
            break;
        default:
            std::cerr << "Invalid option. Please try again.\n" << std::endl;
            pauseScreen();
            continue;
        }
        char continueOption = 'n'; // Default to 'no'
        std::string inputLine;
        std::cout << "Do you want to scan another bios file? (y/n): ";
        std::getline(std::cin, inputLine);
        if (!inputLine.empty()) {
            continueOption = std::tolower(inputLine[0]);
        }
        if (continueOption == 'y') {
            continue;
            biosFileName = "";
            option = '\0';
        } else {
            break;
        }
    } while (true);
}
int main(int argc, char* argv[]) {
    int choice = 0;
    char option = '\0';
    std::string filename = "", biosFileName;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-ami" || std::string(argv[i]) == "ami"){
            choice = 1;
            if (i + 1 < argc)
                filename = argv[++i];
        }
        else if (std::string(argv[i]) == "-vga" || std::string(argv[i]) == "vga"){
            choice = 2;
            if (i + 1 < argc)
                filename = argv[++i];
        }
    }
    if (choice > 0 || filename != "") {
        option = '0' + choice;
        biosFileName = filename;
    }
    checksumCalculator(option, biosFileName);    
    return 0;
}