#pragma once

#include <NetworkManager.h>
#include <stdbool.h>

NMClient *create_client();
bool is_connection_needed(NMClient *client);
