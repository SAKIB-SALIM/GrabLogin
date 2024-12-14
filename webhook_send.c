#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void send_to_webhook(const char *webhook_url, const char *content) {
    CURL *curl;
    CURLcode res;

    // JSON payload with content
    char json_payload[2048];
    snprintf(json_payload, sizeof(json_payload), "{\"content\": \"%s\"}", content);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        // Set the webhook URL
        curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

        // Set the HTTP POST method
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Set the JSON payload
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);

        // Set headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main() {
    const char *webhook_url = "https://discord.com/api/webhooks/1302674995280871545/fsmwXtFfChCn7ktcF3Gy8Pu0mv8YeOv9Izht3yC7Kstm5gHsa8ovmSvepksTpKXc7ICe";
    const char *file_path = "input.txt";
    FILE *file = fopen(file_path, "r");

    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    // Read file contents
    char *content = NULL;
    size_t len = 0;
    fseek(file, 0, SEEK_END);
    len = ftell(file);
    rewind(file);

    content = (char *)malloc(len + 1);
    if (!content) {
        perror("Failed to allocate memory");
        fclose(file);
        return 1;
    }

    fread(content, 1, len, file);
    content[len] = '\0';
    fclose(file);

    // Send the contents to the Discord webhook
    send_to_webhook(webhook_url, content);

    // Free allocated memory
    free(content);

    return 0;
}
