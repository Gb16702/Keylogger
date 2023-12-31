#include <Windows.h>
#include <string>
#include <iostream>

using std::string, std::wstring;

struct registry_key_params {
 	const string& path, name, value;
};

static wstring w_transform_to_wide(const string& input) {
	return wstring(input.begin(), input.end());
}

LONG l_open_registry_key(HKEY& hkey, const std::string& sub_path, REGSAM access) {
	std::wstring w_sub_path = w_transform_to_wide(sub_path);
	return RegOpenKeyEx(HKEY_CURRENT_USER, w_sub_path.c_str(), 0, access, &hkey);
}

void set_registry_key(const registry_key_params& params) {
	using std::cerr;

	HKEY h_key;
	LONG l_result = l_open_registry_key(h_key, params.path, KEY_WRITE);
	if (l_result != ERROR_SUCCESS) {
		cerr << "Error opening registry key: " << l_result << std::endl;
	}
	
	l_result = RegSetValueEx(h_key, w_transform_to_wide(params.name).c_str(), 0, REG_SZ, (const BYTE*)params.value.c_str(), (params.value.size() + 1) * sizeof(char));
	if (l_result != ERROR_SUCCESS) {
		cerr << "Error setting registry value: " << l_result << std::endl;
	}
}
