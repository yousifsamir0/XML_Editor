#!/bin/sh
set -e
echo "Checking for clang-tidy..."
if command -v clang-tidy >/dev/null 2>&1; then
  echo "Running clang-tidy (may require config)..."
  clang-tidy "Source files/xml_convet_tree.cpp" "Source files/opearations.cpp" -- -IHeaders -std=c++17 || true
else
  echo "clang-tidy not found. Skipping. Install clang-tools for linting."
fi

if command -v cpplint >/dev/null 2>&1; then
  echo "Running cpplint..."
  cpplint --filter=-legal "Source files/xml_convet_tree.cpp" "Source files/opearations.cpp" || true
else
  echo "cpplint not found. (Optional) Install cpplint via pip."
fi

echo "Linter checks completed (where available)."
