#pragma once

#include "using_headers.h"

using namespace std;

namespace console_restaurant
{
	//Функция для хеширования пароля.
	//"под капотом" использует md5 и 
	//уреает результат до 16 символов.
	string password_hash(const string& str);
}