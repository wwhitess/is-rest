#include "dish.h"
#include "user.h"
#include "localization.h"
#include "data_row.h"
#include "password_hash.h"
#include "file.h"

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

namespace console_restaurant
{
    // Эта функция - аналог input из Python.
    // input_thing - это ключ к словарю локализации.
    static string input(const string& input_thing)
    {
        cout << localization.enter << input_thing << ":";
        string result;
        while (result.empty())
        {
            getline(cin, result);
        }
        return result;
    }

    // Указатель на текущего пользователя.
    user* current_user = NULL;
    user guest_user = user(string("guest; guest; guest; guest; guest; none"));

    struct role_commands_collection
    {
        string ru_role;     // Роль для русской локализации.
        string eng_role;    // Роль для английской локализации.

        struct command
        {
            string key;
            typedef void (*func_t)(const vector<string>&);   // Тип указателя на функцию, которая является циклом обработки команды.
            func_t func;
            string commentary;
            int arg_target;

            command(const string& key, func_t func, const string& commentary, int arg_target)
            {
                this->key = key;
                this->func = func;
                this->commentary = commentary;
                this->arg_target = arg_target;
            }
        };

        vector<command> commands;

        role_commands_collection(const string& ru, const string& eng, const vector<command>& commands)
        {
            ru_role = ru;
            eng_role = eng;
            this->commands = commands;
        }

        void exec(const string& key_and_args)
        {
            vector<string> data = parse_row(key_and_args, ' ');
            for (command& current_command : commands)
            {
                if (current_command.key == data[0])
                {
                    current_command.func(data);
                    return;
                }
            }
            cout << "Команда не найдена!" << endl;
        }
    };

    namespace commands
    {
        void saveOrdersToFile()
        {
            ofstream file("orders.txt");
            if (!file.is_open())
            {
                cout << "Ошибка при открытии файла для сохранения заказов." << endl;
                return;
            }

            file << "dish;count;status\n";
            for (const auto& order : orders)
            {
                file << order.dish << ";" << order.quantity << ";новый\n";
            }

            file.close();
        }
        void saveProductsToFile()
        {
            ofstream out("products.txt");
            if (!out)
            {
                cout << "Файл невозможно прочитать" << endl;
                return;
            }

            out << "product;count;price\n";

            for (const auto& pair : products)
            {
                const string& product_name = pair.first;
                const product& prod = pair.second;

                out << product_name << ";" << prod.count << ";" << prod.price << "\n";
            }

            out.close();
        }

        void saveDishes()
        {
            ofstream out("dishes.txt");
            if (out.is_open())
            {
                out << "name;description;gramms;cook_time;price,needed_products\n";
                // Записываем данные о блюдах
                for (const auto& dish : dishes)
                {
                    out << dish.name << ';' << dish.description << ';' << dish.gramms << ';' << dish.cook_time << ';' << dish.price << ';';

                    for (const auto& product : dish.needed_products)
                    {
                        out << product.first << ';' << product.second << ';';
                    }

                    out << '\n';
                }

                out.close();
            }
            else
            {
                cout << "Ошибка при открытии файла для сохранения меню." << endl;
            }
        }

        // Просмотр меню.
        void menu(const vector<string>& args)
        {
            string command;
            cout << endl;

            for (const auto& dish : dishes)
            {
                cout << endl;
                cout << "Блюдо: " << dish.name << endl;
                cout << "Описание: " << dish.description << endl;
                cout << "Граммы: " << dish.gramms << endl;
                cout << "Время приготовления: " << dish.cook_time << endl;
                cout << "Цена: " << dish.price << endl;

                cout << "Состав:" << endl;
                for (const auto& product : dish.needed_products)
                {
                    cout << "- " << product.first << ": " << product.second << endl;
                }

                cout << "--------------------------" << endl;
                cout << endl;
            }
        }

        // Редактирование продуктов.
        void products_imp(const vector<string>& args)
        {
            string product_name;
            int new_count;
            int new_price;

            cout << "Введите имя продукта: ";
            getline(cin, product_name);

            cout << "Введите колличество: ";
            cin >> new_count;

            cout << "Введите цену: ";
            cin >> new_price;

            auto it = products.find(product_name);
            if (it != products.end())
            {
                it->second.count = new_count;
                it->second.price = new_price;

                // Сохраняем изменения в файл
                saveProductsToFile();
            }
            else
            {
                cout << "Продукт ненайден" << endl;
            }
        }

        void show_products(const vector<string>& args) 
        {
            for (const auto& product : products)
            {
                cout << endl;
                cout << "Продукт: " << product.first << endl;
                cout << "Колличество: " << product.second.count << endl;
                cout << "Цена: " << product.second.price << endl;
                cout << "--------------------------" << endl;
                cout << endl;
            }
        }

        // Просмотр пользователей.
        void users_imp(const vector<string>& args)
        {
            while (true)
            {
                cout << endl;
                string command = input("действие (edit, list, exit):\n");

                if (command == "edit")
                {
                    string login = input("логин для его изменения: ");
                    for (user& usr : users)
                    {
                        cout << endl;
                        if (usr.login == login)
                        {
                            usr.role = input("роль: ");

                            usr.family = input("фамилию: ");

                            usr.name = input("имя: ");

                            usr.father_name = input("отчество: ");

                            usr.password = password_hash(input("новый пароль: "));

                            save_users();

                            cout << endl;
                            break;
                        }
                    }
                    if (command != "edit")
                    {
                        cout << "Пользователь ненайден." << endl;
                    }
                }
                else if (command == "list")
                {
                    for (const user& usr : users)
                    {
                        cout << endl;
                        cout << "Логин: " << usr.login << endl;
                        cout << "Роль: " << usr.role << endl;
                        cout << "Фамилия: " << usr.family << endl;
                        cout << "Имя: " << usr.name << endl;
                        cout << "Отчество: " << usr.father_name << endl;
                        cout << "Пароль: " << usr.password << endl;
                        cout << "--------------------------" << endl;
                    }
                }
                else if (command == "exit")
                {
                    break;
                }
                else
                {
                    cout << "Такой команды нет. Попробуйте еще раз" << endl;
                }
            }
        }
        void menu_imp(const vector <string>& args)
        {
            string dishName;
            cout << "Введите название блюда, которое вы хотите отредактировать: ";
            getline(cin, dishName);

            for (auto& dish : dishes)
            {
                if (dish.name == dishName)
                {
                    cout << "Введите новое имя блюда: ";
                    getline(cin, dish.name);

                    cout << "Введите новое описание блюда: ";
                    getline(cin, dish.description);

                    cout << "Введите новое количество граммов: ";
                    getline(cin, dish.gramms);

                    cout << "Введите новое время приготовления: ";
                    getline(cin, dish.cook_time);

                    cout << "Введите новую цену: ";
                    getline(cin, dish.price);

                    dish.needed_products.clear();

                    // Вводим новые продукты
                    string productName;
                    int productCount;
                    while (true)
                    {
                        cout << "Введите название продукта (или введите 'конец', чтобы завершить ввод): ";
                        getline(cin, productName);
                        if (productName == "конец")
                            break;

                        cout << "Введите количество продукта: ";
                        cin >> productCount;
                        cin.ignore(); // Очищаем буфер после считывания числа

                        // Добавляем новый продукт в блюдо
                        dish.needed_products.insert({ productName, productCount });
                    }

                    saveDishes(); // Сохраняем изменения в файле
                    return;
                }
            }
            cout << "Блюдо с не найдено." << endl;
        }
        void add_dish(const vector <string>& args)
        {
            string dishName;
            cout << "Введите название блюда: ";
            getline(cin, dishName);

            // Проверяем, есть ли уже блюдо с таким названием
            for (const auto& dish : dishes)
            {
                if (dish.name == dishName)
                {
                    cout << "Блюдо уже существует." << endl;
                    return;
                }
            }

            string dishDescription;
            cout << "Введите описание блюда: ";
            getline(cin, dishDescription);

            string dishGramms;
            cout << "Введите количество граммов: ";
            getline(cin, dishGramms);

            string dishCookTime;
            cout << "Введите время приготовления: ";
            getline(cin, dishCookTime);

            string dishPrice;
            cout << "Введите цену: ";
            getline(cin, dishPrice);

            string dishData = dishName + ";" + dishDescription + ";" + dishGramms + ";" + dishCookTime + ";" + dishPrice;

            // Запрашиваем у пользователя продукты и их количество для нового блюда
            string productName;
            string productQuantity;

            while (true)
            {
                cout << "Введите название продукта (или 'конец' для завершения): ";
                getline(cin, productName);

                if (productName == "конец")
                {
                    break;
                }

                cout << "Введите количество продукта: ";
                getline(cin, productQuantity);

                // Добавляем продукт и его количество в список продуктов для нового блюда
                dishData += ";" + productName + ";" + productQuantity;
            }

            dish newDish(dishData);

            dishes.push_back(newDish);

            saveDishes();

            cout << endl;
        }
        void add_product(const vector <string>& args) 
        {
            string productName;
            cout << "Введите название продукта: ";
            getline(cin, productName);

            auto it = products.find(productName);
            if (it != products.end())
            {
                cout << "Продукт уже существует." << endl;
            }
            else
            {
                int productCount;
                cout << "Введите количество продукта: ";
                cin >> productCount;
                cin.ignore(); // Очищаем буфер после считывания числа

                int addPrice;
                cout << "Введите цену продукта: ";
                cin >> addPrice;
                cin.ignore();

                product newProduct;
                newProduct.count = productCount;
                newProduct.price = addPrice;
                products.insert({ productName, newProduct });
                saveProductsToFile(); // Сохраняем изменения в файле
                cout << endl;
            }
        }
        void delete_dish(const vector <string>& args)
        {
            string dishName;
            cout << "Введите название блюда, которое вы хотите удалить: ";
            getline(cin, dishName);

            auto it = find_if(dishes.begin(), dishes.end(), [&dishName](const dish& d) {
                return d.name == dishName;
                });

            if (it != dishes.end())
            {
                dishes.erase(it);
                saveDishes(); // Сохраняем изменения в файл
            }
            else
            {
                cout << "Блюдо не найдено." << endl;
            }
        }
        void delete_product(const vector <string>& args)
        {
            string productName;
            cout << "Введите название продукта, который вы хотите удалить: ";
            getline(cin, productName);

            auto it = products.find(productName);
            if (it != products.end())
            {
                products.erase(it);
                saveProductsToFile(); // Сохраняем изменения в файле
            }
            else
            {
                cout << "Продукт не найден." << endl;
            }
        }
        void add_order(const vector<string>& args)
        {
            string dishName;
            cout << "Введите название блюда: ";
            getline(cin, dishName);

            string dishCount;
            cout << "Введите колличество: ";
            cin >> dishCount;

            string dishStatus = "новый";

            string orderData = dishName + ";" + dishCount + ";" + dishStatus;

            
            order newOrder(orderData);

            orders.push_back(newOrder);

            saveOrdersToFile();

            cout << endl;
        }
        void view_order(const vector<string>& args)
        {
            if (orders.empty()) {
                cout << "Заказов нет." << endl;
                return;
            }

            cout << "Список заказов:" << endl;
            for (size_t i = 0; i < orders.size(); ++i) {
                cout << "Заказ #" << (i + 1) << ": " << orders[i].dish << " (количество: " << orders[i].quantity << ")" << " (статус: " << orders[i].status << ")" << endl;
            }
        }
    }

    vector<role_commands_collection> role_commands_collections
    {
        //В роль администратора я внес больший функционал для удобной проверки преподавателем
        role_commands_collection("системный администратор", "system administrator",
        {
            role_commands_collection::command("1", commands::menu, "просмотр меню", 0), //All
            role_commands_collection::command("2", commands::add_dish, "добавление блюда", 0), //Admin
            role_commands_collection::command("3", commands::menu_imp, "редактирование меню", 0), //Admin
            role_commands_collection::command("4", commands::delete_dish, "удаление блюда", 0), //Admin
            role_commands_collection::command("5", commands::show_products, "вывести список продуктов", 0),
            role_commands_collection::command("6", commands::add_product, "добавление продукта", 0), //All
            role_commands_collection::command("7", commands::products_imp, "редактирование продуктов", 0), //Admin
            role_commands_collection::command("8", commands::delete_product, "удаление продукта", 0), //Admin
            role_commands_collection::command("9", commands::users_imp, "редактирование и вывод пользователей", 0) //Admin
        }),
        role_commands_collection("складской", "storager",
        {
            role_commands_collection::command("1", commands::show_products, "вывести список продуктов", 0),
            role_commands_collection::command("2", commands::add_product, "добавление продукта", 0)
        }),
        role_commands_collection("поставщик", "provider",
        {
            role_commands_collection::command("1", commands::show_products, "вывести список продуктов", 0)
        }),
        role_commands_collection("бухгалтер", "booker",
        {
            role_commands_collection::command("1", commands::show_products, "вывести список продуктов", 0),
            role_commands_collection::command("2", commands::menu, "просмотр меню", 0), 
        }),
        role_commands_collection("гость", "guest",
        {
           role_commands_collection::command("1", commands::menu, "просмотр меню", 0),
           role_commands_collection::command("2", commands::add_order, "создать заказ", 0)
        }),
        role_commands_collection("повар", "cook",
        {
           role_commands_collection::command("1", commands::view_order, "просмотр заказов", 0)
        }),
        role_commands_collection("официант", "waiter",
        {
            role_commands_collection::command("1", commands::view_order, "просмотр заказов", 0)
        }),
    };

    void main_loop()
    {
        while (true)
        {
            cout << localization.if_guest << ".\n";
            string login = input(localization.login);
            current_user = nullptr;
            // Ищет пользователя с введённым логином
            // Если не находит - то создаёт
            for (user& usr : users)
            {
                if (usr.login == login)
                {
                    current_user = &usr;
                    break;
                }
            }
            if (login == "guest" || login == "гость")
            {
                current_user = &guest_user;
            }
            else if (current_user == nullptr)
            {
                cout << localization.user_not_exits << endl;
                string create_new_user_yn = input(localization.create_new_user_yn);
                user new_user;
                new_user.login = login;
                new_user.password = password_hash(input(localization.password));
                new_user.family = input(localization.family);
                new_user.name = input(localization.name);
                new_user.father_name = input(localization.father_name);
                cout << localization.avable_roles << ":\n";
                for (string* role_ptr : roles)
                {
                    cout << *role_ptr << endl;
                }
                // Программа будет переспрашивать пользователя каждый раз, когда он введёт несуществующую роль.
                // Булевый флаг был добавлен для возможности выхода из двойного цикла.
                bool is_end_of_role_input = false;
                while (true)
                {
                    new_user.role = input(localization.role);
                    for (string* role_ptr : roles)
                    {
                        if (*role_ptr == new_user.role)
                        {
                            is_end_of_role_input = true;
                            break;
                        }
                    }
                    if (is_end_of_role_input)
                    {
                        break;
                    }
                    cout << localization.role_is_not_exits << ".\n";
                }
                // Добавляет нового пользователя, берёт указатель на него и сохраняет в файл.
                users.push_back(new_user);
                current_user = &users[users.size() - 1];
                save_users();
            }
            else
            {
                // Проверяем на ввод пароля в цикле.
                while (true)
                {
                    string password = password_hash(input(localization.password));
                    if (password != current_user->password)
                    {
                        cout << localization.wrong_password << '!' << endl;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            // После логина или регистрации нового пользователя с ролью
            // его приветствует и выводит подсказки.
            if (current_user->login == "guest")
            {
                cout << localization.hello << localization.guest << '!' << endl;
            }
            else
            {
                cout << localization.hello << current_user->name << '!' << endl;
            }
            cout << localization.for_unlogin << endl;
            cout << localization.for_commands_list << endl;
            // Дальше идёт поиск коллекции комманд для роли этого пользователя.
            role_commands_collection* current_role_commands_collection = nullptr;
            for (role_commands_collection& collection : role_commands_collections)
            {
                if (current_user->role == collection.ru_role || current_user->role == collection.eng_role)
                {
                    current_role_commands_collection = &collection;
                    break;
                }
            }
            if (current_role_commands_collection == nullptr)
            {
                cout << "current_role_commands_collection != nullptr" << endl;
                abort();
            }
            while (true)
            {
                string command_key = input(localization.input_command);
                if (command_key == "help")// Вывод списка доступных команд.
                {
                    for (role_commands_collection::command& command : current_role_commands_collection->commands)
                    {
                        cout << command.key << " - " << command.commentary << endl;
                    }
                }
                else if (command_key == "unlogin")// Выход из аккаунта.
                {
                    break;
                }
                else
                {
                    // Дальше ищет команду в списке команд.
                    // Если команды не найдено, то пишет об этом и переспрашивает.
                    role_commands_collection::command* current_command = nullptr;
                    for (role_commands_collection::command& command : current_role_commands_collection->commands)
                    {
                        if (command.key == command_key)
                        {
                            current_command = &command;
                            break;
                        }
                    }
                    if (current_command == nullptr)
                    {
                        cout << "command not found" << endl;
                        continue;
                    }
                    vector<string> arguments;
                    if (current_command->arg_target == 0)// Если в команде не предполагается ввод аргументов.
                    {
                        current_command->func(arguments);
                    }
                    else
                    {
                        // Ввод аргументов, которые разделены пробелом.
                        string args = input(localization.wrong_argument_count);
                        arguments = parse_row(args, ' ');
                        if (arguments.size() != current_command->arg_target)
                        {
                            cout << "invalid args" << endl;
                            continue;
                        }
                        current_command->func(arguments);
                    }
                }
            }
        }
    }
}
