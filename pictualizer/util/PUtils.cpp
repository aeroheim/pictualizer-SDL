#include "PUtils.h"

namespace PUtils
{
	std::wstring str2wstr(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}

	std::string wstr2str(const std::wstring& wstr)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(wstr);
	}

	bool pathIsImage(const std::string& path)
	{
		std::string ext = std::tr2::sys::path(path).extension();

		if (ext == ".jpg" || ext == ".jpeg" || ext == ".png")
			return true;
		
		return false;
	}

	bool pathIsImage(const std::wstring& wpath)
	{
		std::wstring ext = std::tr2::sys::wpath(wpath).extension();

		if (ext == L".jpg" || ext == L".jpeg" || ext == L".png")
			return true;

		return false;
	}

	bool pathIsMusic(const std::string& path)
	{
		std::string ext = std::tr2::sys::path(path).extension();

		if (ext == ".mp3" || ext == ".flac")
			return true;

		return false;
	}

	bool pathIsMusic(const std::wstring& wpath)
	{
		std::wstring ext = std::tr2::sys::wpath(wpath).extension();

		if (ext == L".mp3" || ext == L".flac")
			return true;

		return false;
	}

	bool pathIsDirectory(const std::string& path)
	{
		return std::tr2::sys::is_directory(std::tr2::sys::status(std::tr2::sys::path(path)));
	}

	bool pathIsDirectory(const std::wstring& wpath)
	{
		return std::tr2::sys::is_directory(std::tr2::sys::status(std::tr2::sys::wpath(wpath)));
	}
}