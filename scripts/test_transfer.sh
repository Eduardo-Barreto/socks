#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Configuration
INTERFACE="127.0.0.1"
PORT="8080"
TEST_DIR="test_files"
OUTPUT_DIR="received_files"

# Create test directories
mkdir -p "$TEST_DIR"
mkdir -p "$OUTPUT_DIR"

# Function to generate random file
generate_random_file() {
    local size=$1
    local filename="$TEST_DIR/random_${size}bytes.bin"
    dd if=/dev/urandom of="$filename" bs=1 count="$size" 2>/dev/null
    echo "$filename"
}

# Function to compare files
compare_files() {
    local file1=$1
    local file2=$2
    if cmp -s "$file1" "$file2"; then
        echo -e "${GREEN}✓ Files match${NC}"
        return 0
    else
        echo -e "${RED}✗ Files do not match${NC}"
        return 1
    fi
}

# Function to run a single test
run_test() {
    local input_file=$1
    local output_file="$OUTPUT_DIR/$(basename "$input_file")"
    local test_name=$2

    echo -e "\n${GREEN}Running test: $test_name${NC}"
    echo "Input file: $input_file"
    echo "Output file: $output_file"

    # Start receiver in background
    ./receiver "$INTERFACE" "$PORT" "$output_file" &
    RECEIVER_PID=$!

    # Wait for receiver to start
    sleep 1

    # Run sender
    ./sender "$INTERFACE" "$PORT" "$input_file"

    # Wait for receiver to finish
    wait $RECEIVER_PID

    # Compare files
    compare_files "$input_file" "$output_file"
}

# Test cases

# 1. Small text file
echo "Hello, World!" >"$TEST_DIR/small.txt"
run_test "$TEST_DIR/small.txt" "Small text file"

# 2. Empty file
touch "$TEST_DIR/empty.txt"
run_test "$TEST_DIR/empty.txt" "Empty file"

# 3. Random files of different sizes
for size in 100 1000 10000 100000; do
    input_file=$(generate_random_file $size)
    run_test "$input_file" "Random file ($size bytes)"
done

# 4. Binary file (executable)
cp /bin/ls "$TEST_DIR/ls.bin"
run_test "$TEST_DIR/ls.bin" "Binary file (executable)"

# 5. Large file (1MB)
input_file=$(generate_random_file 1048576)
run_test "$input_file" "Large file (1MB)"

# Cleanup
echo -e "\n${GREEN}Cleaning up...${NC}"
rm -rf "$TEST_DIR" "$OUTPUT_DIR"

echo -e "\n${GREEN}All tests completed!${NC}"
