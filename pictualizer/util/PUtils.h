#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <direct.h>

namespace PUtils
{
	std::wstring str2wstr(std::string);
	std::string getcwd();
	std::wstring getwstrcwd();
	bool pathIsImage(std::string);
	bool pathIsMusic(std::string);
}