#include "headers.h"
void PARSE_HTML_RES(const char *html, size_t length) {
    // This function extracts and prints the "Name," "Synopsis," "Description," and "Error" data
    // while ignoring HTML tags.

    bool Tag_Inside = false;  // Flag to track whether we're inside an HTML tag
    size_t start = 0;       // Start index of data to be processed

    for (size_t i = 0; i < length; i++) {
        if (html[i] == '<') {
            Tag_Inside = true;
        } else if (html[i] == '>') {
            Tag_Inside = false;
            start = i + 1;
        } else if (!Tag_Inside) {
            // If not inside an HTML tag, print the character
            putchar(html[i]);
        }
    }

    // Print a newline character to separate the extracted data
    putchar('\n');
}
size_t CALLBACK_WRITE(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // This callback function is called by curl_easy_perform
    // when it receives data. We can use it to capture the response.

    // Call the PARSE_HTML_RES function to handle the received data
    PARSE_HTML_RES(ptr, size * nmemb);

    // Return the total size of data received
    return size * nmemb;
}


void resolveAndOpenSocket(const char *domain, int port, const char *url_command) {
    // Variables for DNS resolution
    struct addrinfo HINT, *res, *   RPE;
    struct sockaddr_in *ip;
    char ipstr[10000];
    memset(&HINT, 0, sizeof(struct addrinfo));
    HINT.ai_family = AF_UNSPEC;
    HINT.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(domain, NULL, &HINT, &res) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo(domain, NULL, &HINT, &res)));
        return;
    }

    // Resolve DNS using libcurl
    for (RPE = res; RPE != NULL; RPE = RPE->ai_next) {
        void *addr;

        if (RPE->ai_family == AF_INET) {
            ip = (struct sockaddr_in *)RPE->ai_addr;
            addr = &(ip->sin_addr);
            inet_ntop(RPE->ai_family, addr, ipstr, sizeof(ipstr));
        }
    }

    // Create a socket
    int socket_FD = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_FD == -1) {
        perror("socket");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert the IP address from string to binary form
    if (inet_pton(AF_INET, ipstr, &(server_addr.sin_addr)) <= 0) {
        perror("inet_pton");
        close(socket_FD);
        return;
    }

    // Connect to the server
    if (connect(socket_FD, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(socket_FD);
        return;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing libcurl\n");
        return;
    }

    // Set the URL to the domain you want to resolve
    char url[100000]; // Increase the buffer size as needed
    snprintf(url, sizeof(url), "http://%s:%d/?topic=%s&section=all", ipstr, port, url_command);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CALLBACK_WRITE);

    // Create a buffer to store the response
    char buffer[100000];
    memset(buffer, 0, sizeof(buffer));
    size_t buffer_len = 0;

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CALLBACK_WRITE);

    // Perform the DNS resolution and capture the response
        CURLcode reso_curl = curl_easy_perform(curl);
    if (reso_curl != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(reso_curl));
        curl_easy_cleanup(curl);
        return;
    }

    // Cleanup and close libcurl
    curl_easy_cleanup(curl);

    // Close the socket
    close(socket_FD);
}
