#include <Windows.h>
#include <vector>
#include <iostream>

#include "registry_key_actions.hpp"

std::vector<std::wstring> grab_target_email_addr() {
	std::vector<std::wstring> subkey_names;
	HKEY h_key;
	LONG l_result = l_open_registry_key(h_key, "Software\\Microsoft\\IdentityCRL\\UserExtendedProperties\\", KEY_READ);
	if (l_result != ERROR_SUCCESS) {
		std::wcerr << "Error opening registry key: " << l_result << std::endl;
		return subkey_names;
	}

	DWORD i = 0;
	WCHAR subkey_name[256];
	DWORD subkey_name_size = sizeof(subkey_name) / sizeof(subkey_name[0]);

	while (RegEnumKeyEx(h_key, i, subkey_name, &subkey_name_size, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
		subkey_names.push_back(subkey_name);
		subkey_name_size = sizeof(subkey_name) / sizeof(subkey_name[0]);
		++i;
	}

	RegCloseKey(h_key);
	return subkey_names;
}
