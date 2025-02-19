#include <NetworkManager.h>
#include <gio/gio.h>
#include <glib-object.h>
#include <glib.h>
#include <nm-core-enum-types.h>
#include <nm-dbus-interface.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *CYU_WIFI_ID = "cyu-wifi";

int main() {
  GError *error = NULL;
  NMClient *client = nm_client_new(NULL, &error);
  if (client == NULL) {
    g_printerr("Failed to create client: %s\n", error->message);
    g_error_free(error);
    goto cleanup_without_client;
    return EXIT_FAILURE;
  }

  NMState nm_state = nm_client_get_state(client);
  printf("NetworkManager state: %u\n", nm_state);

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
    goto cleanup;
    return EXIT_FAILURE;
  }

  /// No need to free device
  NMActiveConnection *connection = nm_device_get_active_connection(device);
  if (connection == NULL) {
    g_printerr("No active connection\n");
    goto cleanup;
    return EXIT_FAILURE;
  }
  const char *id = nm_active_connection_get_id(connection);
  NMDeviceState state = nm_device_get_state(device);
  NMConnectivityState connectivity =
      nm_device_get_connectivity(device, AF_INET);
  printf("Connected to \"%s\" with state %u and connectivity %u\n", id, state,
         connectivity);

  if (strcmp(id, CYU_WIFI_ID) != 0) {
    g_printerr("Not connected to %s\n", CYU_WIFI_ID);
    goto cleanup;
    return EXIT_FAILURE;
  }

cleanup:
  g_object_unref(client);
cleanup_without_client:
  return EXIT_SUCCESS;
}
