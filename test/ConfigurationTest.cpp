#include "test.h"
#include "Constants.h"
#include "Configuration.h"
#include <filesystem>
#include <fstream>

// tmp
#include <sstream>

TEST(DefaultConfiguration)
{
  Configuration config;
  ASSERT(config.companyName == "YourCompany");
  ASSERT(config.regenTag == "GENERATED BY CPP-DEPENDENCIES");
  ASSERT(config.versionUsed == CURRENT_VERSION);
  ASSERT(config.cycleColor == "orange");
  ASSERT(config.publicDepColor == "blue");
  ASSERT(config.privateDepColor == "lightblue");
  ASSERT(config.componentLinkLimit == 30);
  ASSERT(config.componentLocLowerLimit == 200);
  ASSERT(config.componentLocUpperLimit == 20000);
  ASSERT(config.fileLocUpperLimit == 2000);
  ASSERT(config.addLibraryAliases.size() == 1);
  ASSERT(config.addLibraryAliases.count("add_library") == 1);
  ASSERT(config.addExecutableAliases.size() == 1);
  ASSERT(config.addExecutableAliases.count("add_executable") == 1);
}

TEST(ReadConfigurationFile)
{
  std::stringstream ss(CONFIG_FILE);
  ss << "versionUsed: 3\n"
     << "companyName: MyCompany\n"
     << "regenTag: MY_REGEN_TAG\n"
     << "cycleColor: brown\n"
     << "publicDepColor: black\n"
     << "privateDepColor: grey\n"
     << "componentLink\\\n"              // Line continuation support
     << "Limit: 2\n"
     << "componentLocLowerLimit: 1\n"
     << "componentLocUpperLimit: 123\n"
     << "fileLocUpperLimit: 567          # could have a comment here\n"
     << "reuseCustomSections: true\n"
     << "blacklist: [\n"
     << "  a.h\n"
     << "  b.h\n"
     << "]\n"
     << "wrongTag: 567\n\n\n";

  Configuration config;
  config.read(ss);
  ASSERT(config.companyName == "MyCompany");
  ASSERT(config.regenTag == "MY_REGEN_TAG");
  ASSERT(config.versionUsed == "3");
  ASSERT(config.cycleColor == "brown");
  ASSERT(config.publicDepColor == "black");
  ASSERT(config.privateDepColor == "grey");
  ASSERT(config.componentLinkLimit == 2);
  ASSERT(config.componentLocLowerLimit == 1);
  ASSERT(config.componentLocUpperLimit == 123);
  ASSERT(config.fileLocUpperLimit == 567);
  ASSERT(config.addLibraryAliases.size() == 1);
  ASSERT(config.addLibraryAliases.count("add_library") == 1);
  ASSERT(config.addExecutableAliases.size() == 1);
  ASSERT(config.addExecutableAliases.count("add_executable") == 1);
  ASSERT(config.blacklist.size() == 2);
  ASSERT(config.blacklist.count("a.h") == 1);
  ASSERT(config.blacklist.count("b.h") == 1);
  ASSERT(config.blacklist.count("stdint.h") == 0);
  ASSERT(config.reuseCustomSections);
}

TEST(ReadConfigurationFile_Aliases)
{
  std::stringstream ss(CONFIG_FILE);
  ss << "addLibraryAlias: [\n"
     << "      add_special_library  \n"
     << "      add_test_lib\n"
     << "     ]\n"
     << "addExecutableAlias: [\n"
     << "         add_special_exe\n"
     << "add_test\n"
     << "]\n";

  Configuration config;
  config.read(ss);
  ASSERT(config.addLibraryAliases.size() == 3);
  ASSERT(config.addLibraryAliases.count("add_library") == 1);
  ASSERT(config.addLibraryAliases.count("add_special_library") == 1);
  ASSERT(config.addLibraryAliases.count("add_test_lib") == 1);

  ASSERT(config.addExecutableAliases.size() == 3);
  ASSERT(config.addExecutableAliases.count("add_executable") == 1);
  ASSERT(config.addExecutableAliases.count("add_special_exe") == 1);
  ASSERT(config.addExecutableAliases.count("add_test") == 1);
}

TEST(ReadConfigurationFile_LicenseString)
{
  std::string licenseString(
      "My license\n"
      "It has multiple lines\n"
      "\n"
      "  including whitespace  \n");

  std::stringstream ss(CONFIG_FILE);
  ss << "licenseString: \"\"\"\n"
      << licenseString
      << "  \"\"\"\n";

  Configuration config;
  config.read(ss);
  ASSERT(config.licenseString == licenseString);
}
