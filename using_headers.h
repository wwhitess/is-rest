#pragma once

//Для того, чтобы постоянно не писать кучу include, 
//я просто создам один заголовок со всеми включениями, 
//которые должны быть во всех файлах.

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

//Но иногда они криво работают)))


//У нас везде будет использоваться кодировка UTF-16.
//Для этого замещаем типы string её UTF-16 аналог.
using namespace std;

namespace console_restaurant
{
	using namespace std;

	//using std::string;
	//using std::ifstream;
	//using std::ofstream;
	//using std::cout;
	//using std::cin;
	//using std::endl;
	//using std::vector;
}