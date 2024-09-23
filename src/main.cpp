#include <iostream>
#include <string>
#include <fstream>
#include "../include/confusion.h"
#include "../include/diffusion.h"

#define PROCESS_BLOCK_SIZE 16
#define DEFAULT_ROUNDS 10

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4 || argc > 5) {
    cout << "Usage: ./main <encrypt/decrypt> <message> <key> [rounds]" << endl;
    return 1;
  }

  // First argument is the mode, second is the message, third is the key
  std::string mode = argv[1];
  std::string message = argv[2];
  std::string key = argv[3];
  
  bool encrypting = (mode == "encrypt");

  // Set number of rounds, default to 10 if not specified
  int rounds = DEFAULT_ROUNDS;
  if (argc == 5) {
    rounds = std::stoi(argv[4]);
  }

  // Read the key file
  std::ifstream keyfile(key);
  if (!keyfile.is_open()) {
    cout << "Error: Could not open key file" << endl;
    return 1;
  }

  std::ifstream messagefile(message, std::ios::binary);  // Open in binary mode
  if (!messagefile.is_open()) {
    cout << "Error: Could not open message file" << endl;
    return 1;
  }

  char keychar[PROCESS_BLOCK_SIZE];
  keyfile.read(keychar, PROCESS_BLOCK_SIZE);
  keyfile.close();

  char messagechar[PROCESS_BLOCK_SIZE];
  char temp[PROCESS_BLOCK_SIZE];
  char output[PROCESS_BLOCK_SIZE];
  
  // Read and process 16 bytes at a time
  while (messagefile) {
      messagefile.read(messagechar, PROCESS_BLOCK_SIZE);  // Read up to 16 bytes
      std::streamsize bytesRead = messagefile.gcount();  // Check how many bytes were read
      
      if (bytesRead > 0) {
          bool lastBlock = !!messagefile;
          // If less than 16 bytes were read (e.g., last block), pad the message with 0s
          if (bytesRead < PROCESS_BLOCK_SIZE) {
            // Pad with 0s
            std::fill(messagechar + bytesRead, messagechar + PROCESS_BLOCK_SIZE, 0);
          }
          
          if (encrypting) {
              memcpy(output, messagechar, PROCESS_BLOCK_SIZE);
              for (int i = 0; i < rounds; i++) {
                  diffuse_1(output, keychar, temp);
                  confuse_1(temp, keychar, output);
                  diffuse_2(output, keychar, temp);
                  confuse_2(temp, keychar, output);
              }
              std::cout.write(output, PROCESS_BLOCK_SIZE);
          } else {
              memcpy(output, messagechar, PROCESS_BLOCK_SIZE);
              for (int i = 0; i < rounds; i++) {
                  deconfuse_2(output, keychar, temp);
                  dediffuse_2(temp, keychar, output);
                  deconfuse_1(output, keychar, temp);
                  dediffuse_1(temp, keychar, output);
              }
              if (lastBlock) {
                // before printing, remove nullBytes from the end of the message
                int nullBytes = std::count(output, output + PROCESS_BLOCK_SIZE, '\0');
                std::cout.write(output, PROCESS_BLOCK_SIZE - nullBytes);
              } else {
                std::cout.write(output, PROCESS_BLOCK_SIZE);
              }
          } 
      }
  }
  
  messagefile.close();
  return 0;
}
