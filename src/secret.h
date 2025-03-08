#pragma once

#include <libsecret/secret.h>

struct env {
  const char *username;
  const char *password;
};
typedef struct env env_t;

env_t load_secret();
void store_secret(env_t env);
env_t ask_secret();
