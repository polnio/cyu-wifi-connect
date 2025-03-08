#include "connect.h"
#include "config.h"
#include "secret.h"
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  return size * nmemb;
}

void send_request(env_t *env) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    g_printerr("Failed to init curl\n");
    exit(EXIT_FAILURE);
  }

  char *username = curl_easy_escape(curl, env->username, strlen(env->username));
  char *password = curl_easy_escape(curl, env->password, strlen(env->password));

  char *body = malloc(43 + strlen(username) + strlen(password));
  sprintf(body, "url=&logout=&time=300&authnum=0&uid=%s&pswd=%s", username,
          password);

  curl_easy_setopt(curl, CURLOPT_URL, CYU_PORTAL_URL);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    g_printerr("Failed to send request: %s\n", curl_easy_strerror(res));
    exit(EXIT_FAILURE);
  }

  free(body);
  curl_free(username);
  curl_free(password);
  curl_easy_cleanup(curl);
}
