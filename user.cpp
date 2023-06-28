#include "user.h"
#include "data_row.h"
#include "using_headers.h"
#include "file.h"
#include "localization.h"

using namespace std;

namespace console_restaurant
{
	user::user(const string& data)
	{
		vector<string> parsed_data = parse_row(data);
		//Загрузка полей.
		int index = 0;
		if (parsed_data.size() != 6)
		{
			cout
				<< localization.user_not_valid1
				<< data
				<< localization.user_not_valid2
				<< endl;
			abort();
		}
#define LOAD_FIELD(FIELD) FIELD = parsed_data[index++];
		LOAD_FIELD(role)
			LOAD_FIELD(family)
			LOAD_FIELD(name)
			LOAD_FIELD(father_name)
			LOAD_FIELD(login)
			LOAD_FIELD(password)
#undef LOAD_FIELD
	}

	string user::to_row()
	{
		string result;
		//Сохранение полей.
		result += role;
		result += ';';
		result += family;
		result += ';';
		result += name;
		result += ';';
		result += father_name;
		result += ';';
		result += login;
		result += ';';
		result += password;
		return result;
	}

	vector<user> users;

	vector<string*> roles;

	void load_users()
	{
		string line;    //текущая строка для цикла.
		string header;	//первая "заголовочная" строка со всеми данными о полях. Никак не используется.

		ifstream in = nopen_ifile("users.txt");
		getline(in, header);
		while (getline(in, line))
		{
			users.push_back(user(line));//Заполняем массив пользователями из файла.
		}
	}

	void save_users()
	{
		string write_buffer;
		write_buffer;
		write_buffer += "role;family;name;father_name;login;password\n";
		for (user& usr : users)
		{
			write_buffer += usr.to_row();
			write_buffer += '\n';
		}
		write_buffer.resize(write_buffer.size() - 1);//Удаляем последний перенос строки.

		nwrite_textfile("users.txt", write_buffer);
	}
}