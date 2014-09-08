#!/bin/sh
# autoignore - Create a .gitignore file for automake generated files

git ls-files -o > .gitignore
echo "build/*" >> .gitignore