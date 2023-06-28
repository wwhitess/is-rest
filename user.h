#pragma once

#include "using_headers.h"

using namespace std;

namespace console_restaurant
{
    struct user
    {
        string role;        //роль
        string family;      //фамилия
        string name;        //имя
        string father_name; //отчество
        string login;       //логин
        string password;    //хеш пароля

        user() = default;

        user(const string& data);

        //Превращает пользователя в строку для записи.
        string to_row();
    };

    extern vector<user> users;

    extern vector<string*> roles;//Инициализируется в localization.cpp

    //Функция для загрузки пользователей из файла.
    void load_users();

    void save_users();
}