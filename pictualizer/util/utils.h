#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <direct.h>

namespace utils
{
	std::wstring str2wstr(std::string);
	std::wstring getwstrcwd();
	bool pathIsImage(std::string);
	bool pathIsMusic(std::string);
}