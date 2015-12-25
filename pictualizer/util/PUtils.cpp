#pragma once

#include "PUtils.h"
#include <locale>
#include <codecvt>

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

	bool pathIsImage(const boost::filesystem::path& path)
	{
		std::wstring ext = path.extension().wstring();

		if (ext == L".jpg" || ext == L".jpeg" || ext == L".png")
			return true;
		
		return false;
	}

	bool pathIsMusic(const boost::filesystem::path& path)
	{
		std::wstring ext = path.extension().wstring();

		if (ext == L".mp3" || ext == L".mp2" || ext == L".mp1" || ext == L".wav" || ext == L".ogg" || ext == L".aiff" || ext == L".flac")
			return true;

		return false;
	}

	bool pathIsDirectory(const boost::filesystem::path& path)
	{
		return boost::filesystem::is_directory(path);
	}
}