#include "password_hash.h"
#include "md5.h"

using namespace std;

namespace console_restaurant
{
	string password_hash(const string& str)
	{
		string result = md5(str);
		if (result.size() > 16)
		{
			result.resize(16);
		}
		return result;
	}
}