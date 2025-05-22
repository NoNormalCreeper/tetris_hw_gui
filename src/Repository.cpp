//
// Created by Rikka on 2025/5/21.
//

#include "Repository.h"


void Repository::read() {
}

void Repository::write() {
}

Repository::Repository(const std::string &file_path) {
    this->file_path = file_path;
    this->data = std::nullopt; // Initialize data as null
}
