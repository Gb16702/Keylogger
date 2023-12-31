#ifndef __SET_REGISTRY_KEY__
#define __SET_REGISTRY_KEY__

#include <Windows.h>
#include <iostream>

using std::string;

struct registry_key_params {
	const string& path, name;
	string value;
};

LONG l_open_registry_key(HKEY& hkey, const std::string& sub_path, REGSAM access);

void set_registry_key(const registry_key_params& params);

#endif // !__SET_REGISTRY_KEY__
