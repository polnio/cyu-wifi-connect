#include "config.h"
#include "connect.h"
#include "connectivity.h"
#include "secret.h"
#include <curl/curl.h>

int main() {
  curl_global_init(CURL_GLOBAL_SSL);
  env_t env = load_secret();
  if (env.username == NULL || env.password == NULL) {
    env = ask_secret();
    store_secret(env);
  }
  NMClient *client = create_client();
  if (!is_connection_needed(client)) {
    g_print("No need to connect to portal\n");
    return EXIT_SUCCESS;
  }
  send_request(&env);

  nm_client_check_connectivity(client, NULL, NULL);

  g_print("Connected to " CYU_WIFI_ID "\n");

  curl_global_cleanup();
  return EXIT_SUCCESS;
}
