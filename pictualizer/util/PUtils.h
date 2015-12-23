#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <boost/filesystem.hpp>

namespace PUtils
{
	std::wstring str2wstr(const std::string& str);
	std::string wstr2str(const std::wstring& wstr);

	bool pathIsImage(const boost::filesystem::path& path);
	bool pathIsMusic(const boost::filesystem::path& path);
	bool pathIsDirectory(const boost::filesystem::path& path);
}