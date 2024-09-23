# Encryption/Decryption Program

This program provides functionality to encrypt or decrypt messages using a specific algorithm.

## Compilation

To compile the program, use the following command:

```
g++ -std=c++11 -o bin/encryption_program src/main.cpp src/confusion.cpp src/diffusion.cpp
```

## Usage

To run the program, use the following command:

```
./bin/encryption_program [mode] [input_file] [key]
```

### Arguments

- `command`: Specifies the command to run. Use `encrypt` for encryption and `decrypt` for decryption.
- `input_file`: The path to the input file containing the message to encrypt or decrypt.
- `key`: The encryption/decryption key file.

### Example

To encrypt a message and save the output to a file:

```
./bin/encryption_program encrypt message.txt key.txt > encrypted_message.txt
```

To decrypt a message and save the output to a file:

```
./bin/encryption_program decrypt encrypted_message.txt key.txt > decrypted_message.txt
```

## Notes

- The program expects the key file to contain exactly 16 characters.
- The program processes data in blocks of 16 bytes (128 bits).
- The program uses the confusion and diffusion techniques to secure the data.

## Automated Testing

Create a directory in the `tests` folder for your test case.

Add a file `message.txt` with the message you want to encrypt/decrypt.

Add a file `key.txt` with the encryption/decryption key.

Run the tests with the `compile-and-test.sh` script.
