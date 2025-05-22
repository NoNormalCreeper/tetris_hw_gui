//
// Created by Rikka on 2025/5/21.
//

#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <map>
#include <optional>
#include <string>

using RepoData = struct {}; // TODO: Define the structure of RepoData

class Repository {
public:
    std::string file_path;
    std::optional<RepoData> data;

    void read();
    void write();

    explicit Repository(const std::string& file_path);
    ~Repository() = default;
};

#endif // REPOSITORY_H
