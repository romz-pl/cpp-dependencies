#include "test.h"
#include "TestUtils.h"

#include <fstream>
#include <sstream>

#include "CmakeRegen.h"
#include "Component.h"
#include "Configuration.h"

TEST(MakeCmakeComment_Oneline) {
  const std::string contents("A oneline comment");
  const std::string expectedComment("# A oneline comment\n");

  std::string comment;
  MakeCmakeComment(comment, contents);
  ASSERT(comment == expectedComment);
}

TEST(MakeCmakeComment_Multiline) {
  const std::string contents("Multiple\n"
                             "lines\n"
                             "in\n"
                             "comment");
  const std::string expectedComment("# Multiple\n"
                                    "# lines\n"
                                    "# in\n"
                                    "# comment\n");

  std::string comment;
  MakeCmakeComment(comment, contents);
  ASSERT(comment == expectedComment);
}

TEST(MakeCmakeComment_Multiline_LastWithEndLine) {
  const std::string contents("Multiple\n"
                             "lines\n"
                             "in\n"
                             "comment\n");
  const std::string expectedComment("# Multiple\n"
                                    "# lines\n"
                                    "# in\n"
                                    "# comment\n");

  std::string comment;
  MakeCmakeComment(comment, contents);
  ASSERT(comment == expectedComment);
}

TEST(RegenerateCmakeHeader) {
  Configuration config;
  config.companyName = "My Company";
  config.licenseString = "My\nmultiline\nlicense.";
  config.regenTag = "AUTOGENERATED";

  const std::string expectedOutput(
      "#\n"
      "# Copyright (c) My Company. All rights reserved.\n"
      "# My\n"
      "# multiline\n"
      "# license.\n"
      "#\n"
      "\n"
      "# AUTOGENERATED - do not edit, your changes will be lost\n"
      "# If you must edit, remove these two lines to avoid regeneration\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeHeader(oss, config);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddDependencies) {
  Component comp("./MyComponent");
  comp.buildAfters.insert("ComponentA");
  comp.buildAfters.insert("ComponentB");

  const std::string expectedOutput(
      "add_dependencies(${PROJECT_NAME}\n"
      "  ComponentA\n"
      "  ComponentB\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeAddDependencies(oss, comp);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddDependencies_Empty) {
  Component comp("./MyComponent");

  std::ostringstream oss;
  RegenerateCmakeAddDependencies(oss, comp);

  ASSERT(oss.str().empty());
}

TEST(RegenerateCmakeTargetIncludeDirectories_PublicPrivate) {
  bool isHeaderOnly = false;
  const std::set<std::string> publicIncl{
    "public_include_1",
    "public_include_2"
  };
  const std::set<std::string> privateIncl{
    "private_include_1",
    "private_include_2"
  };

  const std::string expectedOutput(
      "target_include_directories(${PROJECT_NAME}\n"
      "  PUBLIC\n"
      "    public_include_1\n"
      "    public_include_2\n"
      "  PRIVATE\n"
      "    private_include_1\n"
      "    private_include_2\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetIncludeDirectories(oss, publicIncl, privateIncl, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetIncludeDirectories_Public) {
  bool isHeaderOnly = false;
  const std::set<std::string> publicIncl{
    "public_include_1",
    "public_include_2"
  };
  const std::set<std::string> privateIncl{};
  const std::string expectedOutput(
      "target_include_directories(${PROJECT_NAME}\n"
      "  PUBLIC\n"
      "    public_include_1\n"
      "    public_include_2\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetIncludeDirectories(oss, publicIncl, privateIncl, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetIncludeDirectories_Private) {
  bool isHeaderOnly = false;
  const std::set<std::string> publicIncl{};
  const std::set<std::string> privateIncl{
    "private_include_1",
    "private_include_2"
  };

  const std::string expectedOutput(
      "target_include_directories(${PROJECT_NAME}\n"
      "  PRIVATE\n"
      "    private_include_1\n"
      "    private_include_2\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetIncludeDirectories(oss, publicIncl, privateIncl, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetIncludeDirectories_Interface) {
  bool isHeaderOnly = true;
  const std::set<std::string> publicIncl{
    "public_include_1",
    "public_include_2"
  };
  const std::set<std::string> privateIncl{
    "private_include_1",
    "private_include_2"
  };

  const std::string expectedOutput(
      "target_include_directories(${PROJECT_NAME}\n"
      "  INTERFACE\n"
      "    public_include_1\n"
      "    public_include_2\n"
      "    private_include_1\n"
      "    private_include_2\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetIncludeDirectories(oss, publicIncl, privateIncl, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetLinkLibraries_PublicPrivate) {
  bool isHeaderOnly = false;
  const std::set<std::string> publicDeps{
    "ComponentA",
    "ComponentB"
  };
  const std::set<std::string> privateDeps{
    "ComponentZ",
    "ComponentY"
  };

  const std::string expectedOutput(
      "target_link_libraries(${PROJECT_NAME}\n"
      "  PUBLIC\n"
      "    ComponentA\n"
      "    ComponentB\n"
      "  PRIVATE\n"
      "    ComponentY\n"
      "    ComponentZ\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetLinkLibraries(oss, publicDeps, privateDeps, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetLinkLibraries_Public) {
  bool isHeaderOnly = false;
  const std::set<std::string> publicDeps{
    "ComponentA",
    "ComponentB"
  };
  const std::set<std::string> privateDeps{};

  const std::string expectedOutput(
      "target_link_libraries(${PROJECT_NAME}\n"
      "  PUBLIC\n"
      "    ComponentA\n"
      "    ComponentB\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetLinkLibraries(oss, publicDeps, privateDeps, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetLinkLibraries_Private) {
  bool isHeaderOnly = false;
  const std::set<std::string> publicDeps{};
  const std::set<std::string> privateDeps{
    "ComponentZ",
    "ComponentY"
  };

  const std::string expectedOutput(
      "target_link_libraries(${PROJECT_NAME}\n"
      "  PRIVATE\n"
      "    ComponentY\n"
      "    ComponentZ\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetLinkLibraries(oss, publicDeps, privateDeps, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeTargetLinkLibraries_Interface) {
  bool isHeaderOnly = true;
  const std::set<std::string> publicDeps{
    "ComponentA",
    "ComponentB"
  };
  const std::set<std::string> privateDeps{
    "ComponentZ",
    "ComponentY"
  };

  const std::string expectedOutput(
      "target_link_libraries(${PROJECT_NAME}\n"
      "  INTERFACE\n"
      "    ComponentA\n"
      "    ComponentB\n"
      "    ComponentY\n"
      "    ComponentZ\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeTargetLinkLibraries(oss, publicDeps, privateDeps, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddTarget_Interface) {
  Component comp("./MyComponent/");
  comp.type = "add_library";
  comp.additionalTargetParameters = "  EXCLUDE_FROM_ALL\n";

  Configuration config;

  bool isHeaderOnly = true;

  const std::list<std::string> files{
    "Analysis.cpp",
    "Analysis.h",
    "main.cpp"
  };

  const std::string expectedOutput(
      "add_library(${PROJECT_NAME} INTERFACE\n"
      "  EXCLUDE_FROM_ALL\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeAddTarget(oss, config, comp, files, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddTarget_Library) {
  Component comp("./MyComponent/");
  comp.type = "add_library";
  comp.additionalTargetParameters = "  EXCLUDE_FROM_ALL\n";

  Configuration config;

  bool isHeaderOnly = false;

  const std::list<std::string> files{
    "Analysis.cpp",
    "Analysis.h",
    "main.cpp"
  };

  const std::string expectedOutput(
      "add_library(${PROJECT_NAME} STATIC\n"
      "  Analysis.cpp\n"
      "  Analysis.h\n"
      "  main.cpp\n"
      "  EXCLUDE_FROM_ALL\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeAddTarget(oss, config, comp, files, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddTarget_LibraryAlias) {
  Component comp("./MyComponent/");
  comp.type = "add_library_alias";
  comp.additionalTargetParameters = "  EXCLUDE_FROM_ALL\n";

  Configuration config;
  config.addLibraryAliases.insert("add_library_alias");

  bool isHeaderOnly = false;

  const std::list<std::string> files{
    "Analysis.cpp",
    "Analysis.h",
    "main.cpp"
  };

  const std::string expectedOutput(
      "add_library_alias(${PROJECT_NAME} STATIC\n"
      "  Analysis.cpp\n"
      "  Analysis.h\n"
      "  main.cpp\n"
      "  EXCLUDE_FROM_ALL\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeAddTarget(oss, config, comp, files, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddTarget_Executable) {
  Component comp("./MyComponent/");
  comp.type = "add_executable";
  comp.additionalTargetParameters = "  EXCLUDE_FROM_ALL\n";

  Configuration config;

  bool isHeaderOnly = false;

  const std::list<std::string> files{
    "Analysis.cpp",
    "Analysis.h",
    "main.cpp"
  };

  const std::string expectedOutput(
      "add_executable(${PROJECT_NAME}\n"
      "  Analysis.cpp\n"
      "  Analysis.h\n"
      "  main.cpp\n"
      "  EXCLUDE_FROM_ALL\n"
      ")\n"
      "\n");

  std::ostringstream oss;
  RegenerateCmakeAddTarget(oss, config, comp, files, isHeaderOnly);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddSubdirectory_NoSubDirs) {
  TemporaryWorkingDirectory workdir(name);
  std::filesystem::create_directories(workdir() / "MyComponent");

  Component comp("./MyComponent/");

  const std::string expectedOutput("");

  std::ostringstream oss;
  RegenerateCmakeAddSubdirectory(oss, comp);

  ASSERT(oss.str() == expectedOutput);
}

TEST(RegenerateCmakeAddSubdirectory_SubDirsWithAndWithoutCmakeLists) {
  TemporaryWorkingDirectory workdir(name);
  std::filesystem::create_directories(workdir() / "MyComponent");
  std::filesystem::create_directories(workdir() / "MyComponent" / "SubComponentA");
  std::ofstream((workdir() / "MyComponent" / "SubComponentA" / "CMakeLists.txt").c_str()).close();
  std::filesystem::create_directories(workdir() / "MyComponent" / "SubComponentB");
  std::ofstream((workdir() / "MyComponent" / "SubComponentB" / "CMakeLists.txt").c_str()).close();
  std::filesystem::create_directories(workdir() / "MyComponent" / "Data");

  Component comp("./MyComponent/");

  const std::string expectedOutput(
    "add_subdirectory(SubComponentA)\n"
    "add_subdirectory(SubComponentB)\n");

  std::ostringstream oss;
  RegenerateCmakeAddSubdirectory(oss, comp);

  ASSERT(oss.str() == expectedOutput);
}
