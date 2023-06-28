#include "localization.h"

#include <map>

using namespace std;

namespace console_restaurant
{
	localization_t localization;

	extern vector<string*> roles;

	void sub_init()
	{
		roles =
		{
			&localization.sys_admin,
			&localization.storager,
			&localization.provider,
			&localization.booker,
			&localization.cook,
			&localization.officiant
		};
	}

	void init_rus_localization()
	{
		localization.product_not_valid1 = "Продукт ";
		localization.product_not_valid2 = " не валиден.";
		localization.dish_not_valid1 = "Блюдо ";
		localization.dish_not_valid2 = " не валидно.";
		localization.user_not_valid1 = "Пользователь ";
		localization.user_not_valid2 = " не валиден.";
		localization.enter = "Введите ";
		localization.login = "логин";
		localization.hello = "Здравствуйте ";
		localization.password = "пароль";
		localization.family = "фамилию";
		localization.name = "имя";
		localization.father_name = "отчество";
		localization.role = "роль";
		localization.user_not_exits = "Пользователя с таким логином не существует.";
		localization.create_new_user_yn = "Создать нового пользователя? Напишите да/нет";
		localization.y = "д";
		localization.n = "н";
		localization.cant_open_file = "Не удалось открыть файл ";
		localization.if_guest = "Если хотите войти как гость, то вводите \"гость\".";
		localization.avable_roles = "Доступные роли";
		localization.role_is_not_exits = "Такой роли нету";
		localization.for_unlogin = "Для выхода из аккаунта введите \"unlogin\"";
		localization.for_commands_list = "Для просмотра списка комманд введите \"help\"";
		localization.cant_find_command = "Не удалось найти комманду";
		localization.input_command = "комманду";
		localization.wrong_password = "Неправильный пароль";
		localization.wrong_argument_count = "Неправильное количество аргументов";
		localization.guest = "гость";

		localization.sys_admin = "системный администратор";
		localization.storager = "складской";
		localization.provider = "поставщик";
		localization.booker = "бухгалтер";
		localization.cook = "повар";
		localization.officiant = "Оффициант";
		sub_init();
	}

	void init_eng_localization()
	{
		localization.product_not_valid1 = "Product ";
		localization.product_not_valid2 = " not valid.";
		localization.dish_not_valid1 = "Dish ";
		localization.dish_not_valid2 = "  not valid.";
		localization.user_not_valid1 = "User ";
		localization.user_not_valid2 = " not valid.";
		localization.enter = "Enter ";
		localization.login = "login";
		localization.hello = "Hello ";
		localization.password = "password";
		localization.family = "family";
		localization.name = "name";
		localization.father_name = "patronim";//Отчество
		localization.role = "роль";
		localization.user_not_exits = "User with this login is not exits.";
		localization.create_new_user_yn = "Create new user? Enter yes/no";
		localization.y = "y";
		localization.n = "n";
		localization.cant_open_file = "Cant open file ";
		localization.if_guest = "If you needed Enter as guest, input \"guest\"";
		localization.avable_roles = "Доступные роли";
		localization.role_is_not_exits = "This role is not exits";
		localization.for_unlogin = "For unlogin input \"unlogin\"";
		localization.for_commands_list = "For watching list of commands input \"help\"";
		localization.cant_find_command = "Cant find command";
		localization.input_command = "command";
		localization.wrong_password = "Wrong password";
		localization.wrong_argument_count = "Wrong argument count";
		localization.guest = "guest";

		localization.sys_admin = "system administrator";
		localization.storager = "storager";
		localization.provider = "provider";
		localization.booker = "booker";
		localization.cook = "cook";
		localization.officiant = "officiant";
		sub_init();
	}
}