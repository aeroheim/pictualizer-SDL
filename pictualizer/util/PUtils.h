#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <filesystem>

namespace PUtils
{
	std::wstring str2wstr(const std::string& str);
	std::string wstr2str(const std::wstring& wstr);

	bool pathIsImage(const std::string& path);
	bool pathIsImage(const std::wstring& wpath);

	bool pathIsMusic(const std::string& path);
	bool pathIsMusic(const std::wstring& wpath);
}