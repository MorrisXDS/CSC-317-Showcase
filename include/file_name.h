//
// Created by morris on 9/19/25.
//

#ifndef RAYTRACING_FILE_NAME_HPP
#define RAYTRACING_FILE_NAME_HPP

#include <string>

using string = std::string;
string extractFilename(const string& path) {
    size_t lastSlash = path.find_last_of("/\\");
    size_t lastDot = path.find_last_of('.');

    size_t start = (lastSlash == string::npos) ? 0 : lastSlash + 1;
    size_t end = (lastDot == string::npos) ? string::npos : lastDot;

    // Make sure dot comes after slash
    if (lastDot != string::npos && lastSlash != string::npos && lastDot < lastSlash) {
        end = string::npos;
    }

    return (end == string::npos) ? path.substr(start) : path.substr(start, end - start);
}

#endif //RAYTRACING_FILE_NAME_HPP