#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

const int MAX_BUFF = 4096;

ssize_t readn(int , std::string &);
ssize_t writen(int, std::string&);