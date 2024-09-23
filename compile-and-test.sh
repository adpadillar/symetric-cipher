#!/bin/bash

# Check if rounds parameter is provided
if [ $# -eq 1 ]; then
    rounds=$1
else
    rounds=""
fi

mkdir -p bin

# Compile the program
g++ -std=c++11 -o bin/encryption_program src/main.cpp src/confusion.cpp src/diffusion.cpp

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi

echo "Compilation successful"

# Function to run test for a given test number
run_test() {
    local test_num=$1
    echo "Running test $test_num..."
    
    # Run encryption
    if [ -n "$rounds" ]; then
        ./bin/encryption_program encrypt tests/$test_num/message tests/$test_num/key $rounds > encrypted_output
    else
        ./bin/encryption_program encrypt tests/$test_num/message tests/$test_num/key $rounds > encrypted_output
    fi
    
    # Run decryption
    if [ -n "$rounds" ]; then
        ./bin/encryption_program decrypt encrypted_output tests/$test_num/key $rounds > decrypted_output
    else
        ./bin/encryption_program decrypt encrypted_output tests/$test_num/key $rounds > decrypted_output
    fi
    
    # Compare original and decrypted files
    if cmp -s tests/$test_num/message decrypted_output; then
        echo "Test $test_num passed: Original and decrypted files match"
    else
        echo "Test $test_num failed: Original and decrypted files do not match"
        exit 1
    fi
    
    # Clean up temporary files
    rm encrypted_output decrypted_output
}

# Run tests for all test directories
for test_dir in tests/*/; do
    test_num=$(basename "$test_dir")
    run_test "$test_num"
done

echo "All tests completed successfully"
