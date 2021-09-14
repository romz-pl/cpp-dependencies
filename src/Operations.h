#pragma once

#include "Analysis.h"
#include "CmakeRegen.h"
#include "Component.h"
#include "Configuration.h"
#include "Constants.h"
#include "FilesystemInclude.h"
#include "FstreamInclude.h"
#include "Input.h"
#include "Output.h"

class Operations {
public:
    Operations(int argc, const char** argv);
    void RunCommands();
    
private:
    typedef void (Operations::*Command)(std::vector<std::string>);
    void RegisterCommands();
    void RunCommand(std::vector<std::string>::iterator &arg, std::vector<std::string>::iterator &end);
    void LoadProject(bool withLoc = false);
    void UnloadProject();
    void Dir(std::vector<std::string> args);
    void Ignore(std::vector<std::string> args);
    void Infer(std::vector<std::string> );
    void Drop(std::vector<std::string> args);
    void Graph(std::vector<std::string> args);
    void GraphCycles(std::vector<std::string> args);
    void GraphTarget(std::vector<std::string> args);
    void Cycles(std::vector<std::string> args);
    void Stats(std::vector<std::string>);
    void InOut(std::vector<std::string> args);
    void Shortest(std::vector<std::string> args);
    void Info(std::vector<std::string> args);
    void UsedBy(std::vector<std::string> args);
    void IncludeOrigin(std::vector<std::string> args);
    void DoActualRegen(std::vector<std::string> args, bool dryRun);
    void Regen(std::vector<std::string> args);
    void DryRegen(std::vector<std::string> args);
    void FixIncludes(std::vector<std::string> args);
    void Outliers(std::vector<std::string>);
    void IncludeSize(std::vector<std::string>);
    void Ambiguous(std::vector<std::string>);
    void Recursive(std::vector<std::string>);
    void Help(std::vector<std::string>);
    
private:
    Configuration config;
    enum LoadStatus {
      Unloaded,
      FastLoad,
      FullLoad,
    } loadStatus;
    bool inferredComponents;
    bool lastCommandDidNothing;
    std::string programName;
    std::map<std::string, Command> commands;
    std::vector<std::string> allArgs;
    std::unordered_map<std::string, Component *> components;
    std::unordered_map<std::string, File> files;
    std::map<std::string, std::set<std::string>> collisions;
    std::unordered_map<std::string, std::string> includeLookup;
    std::map<std::string, std::vector<std::string>> ambiguous;
    std::set<std::string> deleteComponents;
    filesystem::path outputRoot, projectRoot;
    bool recursive;
};

