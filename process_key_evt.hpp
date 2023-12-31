#ifndef __PROCESS_KEY_EVT__
#define __PROCESS_KEY_EVT__

#include<Windows.h>

void process_key_evt(DWORD VK_code, bool is_key_down, const std::string& api_endpoint_url, const std::vector<std::wstring>& subkey_names);

#endif // !__PROCESS_KEY_EVT__
