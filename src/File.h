#pragma once

#include <filesystem>
#include <map>
#include <unordered_set>

// Forward reference:
struct Component;

struct File {
    File(const std::filesystem::path& path);

    void AddIncludeStmt(bool withPointyBrackets, const std::string& filename);
    

    std::filesystem::path path;
    std::map<std::string, bool> rawIncludes;
    std::unordered_set<File *> dependencies;
    std::unordered_set<std::string> includePaths;
    Component *component;
    size_t loc;
    size_t includeCount;
    bool hasExternalInclude;
    bool hasInclude;
};
