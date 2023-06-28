#pragma once

#include "using_headers.h"
#include "localization.h"

using namespace std;

namespace console_restaurant
{
	//Открывает файл для чтения.
	//Если не получается, то сообщает об ошибке 
	//и завершает программу.
	inline ifstream nopen_ifile(const string& path)
	{
		ifstream result(path);
		if (!result.is_open())
		{
			cout << localization.cant_open_file << path << " r\n";
			abort();
		}
		return result;
	}

	//Записывает текст в файл.
	//Старое содержимое удаляется.
	//Если не получается, то сообщает об ошибке 
	//и завершает программу.
	inline void nwrite_textfile(const string& path, const string& data)
	{
		ofstream writer(path);
		if (!writer.is_open())
		{
			cout << localization.cant_open_file << path << " w\n";
			abort();
		}
		writer << data;
		writer.flush();
		writer.close();
	}
}