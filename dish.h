#pragma once

#include "using_headers.h"

#include<map>

using namespace std;

namespace console_restaurant
{
	//Это map, где ключ - это название продукта, а значение - количество продуктов.
	struct product
	{
		int count;
		int price;
	};

	struct dish
	{
		string name;
		string description;
		string gramms;
		string cook_time;
		string price;
		std::map<string, int> needed_products;

		dish(string& data);

		string to_row();
	};

	struct order
	{
		string dish;
		string quantity;
		string status;
	};



	extern vector<order> orders; //Заказы
	extern  std::map<string, product> products;//Продукты на складе.
	extern vector<dish> dishes;//Меню.

	//Функция для загрузки продуктов из файла.
	void load_produts();

	//Функция для загрузки блюд из файла.
	void load_dishes();

	void load_orders();
}