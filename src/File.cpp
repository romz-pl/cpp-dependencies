#include "File.h"


File::File(const std::filesystem::path& path)
: path(path)
, component(NULL)
, loc(0)
, includeCount(0)
, hasExternalInclude(false)
, hasInclude(false)
{
}

void File::AddIncludeStmt(bool withPointyBrackets, const std::string& filename) {
	rawIncludes.insert(std::make_pair(filename, withPointyBrackets));
}
