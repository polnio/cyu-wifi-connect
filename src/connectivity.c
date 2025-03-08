#include "connectivity.h"
#include "config.h"
#include <NetworkManager.h>
#include <glib.h>
#include <stdbool.h>

NMClient *create_client() {
  GError *error = NULL;
  NMClient *client = nm_client_new(NULL, &error);
  if (client == NULL) {
    g_printerr("Failed to create client: %s\n", error->message);
    g_error_free(error);
    exit(EXIT_FAILURE);
  }
  return client;
}

bool is_connection_needed(NMClient *client) {
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
      nm_client_check_connectivity(client, NULL, NULL);
  if (connectivity != NM_CONNECTIVITY_PORTAL) {
    g_print("No need to connect to portal\n");
    return false;
  }
  return true;
}
