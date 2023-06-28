#pragma once

#include "using_headers.h"

using namespace std;

namespace console_restaurant
{
	//Находит индекс заданной строки в массиве.
	int index_of(vector<string>& data, const string& key);

	//Парсит csv ряд в массив строк
	vector<string> parse_row(const string& row, char splitter = ';');

	//Парсит массив строк в строку для записи в файл.
	string to_row(vector<string>& data);

	//У структур могут быть методы to_row.
	//Они предназначены для превращения структуры в строку при помощи функции to_row.
	//Их конструкторы превращают строку в массив при помощи функции parse_row, 
	//а потом массив превращается в структуру.
}