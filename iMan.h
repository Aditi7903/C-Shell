#ifndef  __IMAN_H
#define  __IMAN_H

void PARSE_HTML_RES(const char *html, size_t length) ;
size_t CALLBACK_WRITE(char *ptr, size_t size, size_t nmemb, void *userdata);
void resolveAndOpenSocket(const char *domain, int port, const char *url_command);

#endif
