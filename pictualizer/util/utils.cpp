#include "utils.h"

namespace utils
{
	std::wstring str2wstr(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}	

	std::wstring getwstrcwd()
	{
		const int MAXPATHLEN = 256;
		char temp[MAXPATHLEN];
		return str2wstr(_getcwd(temp, MAXPATHLEN) ? std::string(temp) : std::string(""));
	}
}