#!/bin/sh
set -e
echo "Building tests..."
g++ -std=c++17 -IHeaders tests/test_tree.cpp "Source files/tree_node.cpp" "Source files/tree_parser.cpp" "Source files/tree_render.cpp" -o tests/test_tree
echo "Running tests..."
./tests/test_tree
echo "All tests finished."
