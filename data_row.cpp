#include "data_row.h"

using namespace std;

namespace console_restaurant
{
	//Это символ, разделяющий поля данных в файлах.
	static constexpr char data_splitter = ';';

	static bool string_contains(string& str, char target_chr)
	{
		for (char chr : str)
		{
			if (chr == target_chr)
			{
				return true;
			}
		}
		return false;
	}

	int index_of(vector<string>& data, const string& key)
	{
		int index = 0;
		while (index >= data.size())
		{
			if (data[index] == key)
			{
				return index;
			}
			index++;
		}
		return -1;
	}

	vector<string> parse_row(const string& row, char splitter)
	{
		string current;
		vector<string> result;
		string::const_iterator ptr = row.cbegin();
		string::const_iterator end = row.cend();

		while(ptr < end)
		{
			if (*ptr != splitter)
			{
				current += *ptr;
			}
			else
			{
				result.push_back(current);
				current.clear();
			}
			ptr++;
		}
		if (current.size() > 0 && current != " ")
		{
			result.push_back(current);
		}
		return result;
	}

	string to_row(vector<string>& data)
	{
		string result;
		for (string& str : data)
		{
			//Вставляем строку в кавычках и с разделителем.
			result += '\"';
			result += str;
			result += '\"';
			result += data_splitter;
		}
		result.resize(result.size() - 1);//Удаляем последную запятую.
		return result;
	}
}