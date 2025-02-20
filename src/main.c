#include <NetworkManager.h>
#include <curl/curl.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <nm-core-enum-types.h>
#include <nm-dbus-interface.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  return size * nmemb;
}

#define CYU_WIFI_ID "cyu-wifi"
#define CYU_PORTAL_URL "https://portail-wifi-1.u-cergy.fr/auth/plain.html"

bool is_connection_needed() {
  GError *error = NULL;
  NMClient *client = nm_client_new(NULL, &error);
  if (client == NULL) {
    g_printerr("Failed to create client: %s\n", error->message);
    g_error_free(error);
    exit(EXIT_FAILURE);
  }

  const GPtrArray *devices = nm_client_get_all_devices(client);

  /// No need to free devices
  NMDevice *device = NULL;
  for (guint i = 0; i < devices->len; i++) {
    NMDevice *d = g_ptr_array_index(devices, i);
    if (NM_IS_DEVICE_WIFI(d)) {
      device = d;
      break;
    }
  }
  if (device == NULL) {
    g_printerr("No wifi device found\n");
    exit(EXIT_FAILURE);
  }

  /// No need to free device
  NMActiveConnection *connection = nm_device_get_active_connection(device);
  if (connection == NULL) {
    g_print("No active connection\n");
    return false;
  }
  const char *id = nm_active_connection_get_id(connection);

  if (strcmp(id, CYU_WIFI_ID) != 0) {
    g_print("Not connected to " CYU_WIFI_ID "\n");
    return false;
  }

  NMConnectivityState connectivity =
      nm_device_get_connectivity(device, AF_INET);
  if (connectivity != NM_CONNECTIVITY_PORTAL) {
    g_print("No need to connect to portal\n");
    return false;
  }
  return true;
}

struct env {
  const char *username;
  const char *password;
};
typedef struct env env_t;

env_t load_env() {
  const char *username = g_getenv("CYU_WIFI_USERNAME");
  if (username == NULL) {
    g_printerr("No username found\n");
    exit(EXIT_FAILURE);
  }
  const char *password = g_getenv("CYU_WIFI_PASSWORD");
  if (password == NULL) {
    g_printerr("No password found\n");
    exit(EXIT_FAILURE);
  }
  env_t env = {username, password};
  return env;
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

int main() {
  curl_global_init(CURL_GLOBAL_SSL);
  env_t env = load_env();
  if (!is_connection_needed()) {
    g_print("No need to connect to portal\n");
    return EXIT_SUCCESS;
  }
  send_request(&env);

  g_print("Connected to " CYU_WIFI_ID "\n");

  curl_global_cleanup();
  return EXIT_SUCCESS;
}
