#include <windows.h>
#include "using_headers.h"
#include "user.h"
#include "dish.h"
#include "localization.h"
#include "main_loop.h"

#include <filesystem>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

using namespace std;

using namespace console_restaurant;

int main()
{
	//переносим рабочий каталог (cwd) в папку data.
	u8string target_cwd = filesystem::current_path().u8string();
	target_cwd += u8"\\data";
	filesystem::current_path(target_cwd);
	//Спрашиваем локализацию
	//А также устанавливаем кодировку на UTF-16.
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	_setmode(_fileno(stdout), _O_TEXT);
	_setmode(_fileno(stdin), _O_TEXT);
	while (true)
	{
		cout
			<< "By S.Lebedev" << endl
			<< "For english localization type \"eng\"\n"
			<< "Для русской локализации введите \"рус\"\n";
		string locale;
		cin >> locale;
		if (locale == "eng")
		{
			init_eng_localization();
			break;
		}
		else if (locale == "рус")
		{
			init_rus_localization();
			break;
		}
	}
	//Загружаем пользователей, продукты, блюда.
	load_users();
	load_produts();
	load_dishes();
	load_orders();
	main_loop();
}