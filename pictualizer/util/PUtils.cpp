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

		if (extension.compare(".JPG") == 0 || extension.compare(".jpg") == 0 || extension.compare(".PNG") == 0 || extension.compare(".png") == 0 || 
			extension.compare(".JPEG") == 0 || extension.compare(".jpeg") == 0)
			return true;
		
		return false;
	}

	bool pathIsMusic(std::string path)
	{
		size_t extensionIndex = path.find_last_of(".", path.length() - 1);
		std::string extension = path.substr(extensionIndex);
		
		if (extension.compare(".MP3") == 0 || extension.compare(".mp3") == 0)
			return true;

		return false;
	}
}