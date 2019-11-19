//
// Created by mhennefarth on 11/18/19.
//

#ifndef VECTORFIELDANALYSIS_UTILITIES_H
#define VECTORFIELDANALYSIS_UTILITIES_H

#include <vector>
#include <string>

#define PI 3.14159265

std::vector<std::string> split(const std::string &val, const char &delim, std::vector<std::string> *out=nullptr){
    if(out == nullptr){
        std::vector<std::string> result;
        out = &result;
    }
    std::string::size_type start = 0;
    for(std::string::size_type i = 0; i < val.size(); i++){
        if(val[i] == delim && start != i){
            out->push_back(val.substr(start, i-start));
            start = i+1;
        }
    }
    out->push_back(val.substr(start, val.size()));

    //Now we filer out
    std::string checker(1, delim);
    for(std::vector<std::string>::size_type i = 0; i < out->size(); i++){
        if (out->at(i) == checker){
            out->erase(out->begin() + i);
            i--;
        }
    }
    return *out;
}

bool contains(const std::string &val, const std::string &key){
    return val.find(key) != std::string::npos;
}

bool contains(const std::vector<std::string> &list, const std::string &key){
    for(std::vector<std::string>::size_type i = 0; i < list.size(); i++)
        if(key == list[i])
            return true;

    return false;
}

std::string& remove(std::string &key, char delim){
    std::string result;
    for(std::string::size_type i = 0; i < key.size(); i++) {
        if(key[i] != delim){
            result += key[i];
        }
    }
    key = result;
    return key;
}

std::string join(const std::vector<std::string> &list){
    std::string result;

    for(std::string::size_type i = 0; i < list.size(); i++){
        result += list[i];
    }
    return result;
}

std::string replace(const std::string &str, const char &key, const char &rep){

    std::string result = "";

    for(std::string::size_type i = 0; i < str.size(); i++){
        if (str[i] == key){
            result += rep;
        }
        else{
            result += str[i];
        }
    }
    return result;
}

std::string& replace(std::string &str, const char &key, const char &rep, const bool &inplace){
    if(inplace){
        for(std::string::size_type i = 0; i < str.size(); i++){
            if(str[i] == key){
                str[i] = rep;
            }
        }
        return str;
    }
}

#endif //VECTORFIELDANALYSIS_UTILITIES_H
