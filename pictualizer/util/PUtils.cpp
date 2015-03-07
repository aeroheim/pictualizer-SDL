#include "PUtils.h"

namespace PUtils
{
	std::wstring str2wstr(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	std::string getcwd()
	{
		const int MAXPATHLEN = 256;
		char temp[MAXPATHLEN];
		return _getcwd(temp, MAXPATHLEN) ? std::string(temp) : std::string("");
	}

	std::wstring getwstrcwd()
	{
		return str2wstr(getcwd());
	}

	bool pathIsImage(std::string path)
	{
		size_t extensionIndex = path.find_last_of(".", path.length() - 1);
		std::string extension = path.substr(extensionIndex);

		if (extension.compare(".JPG") || extension.compare(".jpg") || extension.compare(".PNG") || extension.compare(".png")
			|| extension.compare(".JPEG") || extension.compare(".jpeg"))
			return true;
		
		return false;
	}

	bool pathIsMusic(std::string path)
	{
		size_t extensionIndex = path.find_last_of(".", path.length() - 1);
		std::string extension = path.substr(extensionIndex);

		// fix this later
		return false;
	}
}