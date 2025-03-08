#include "secret.h"
#include <stdio.h>
#include <termios.h>

enum TYPE { USERNAME, PASSWORD };
inline const SecretSchema *get_schema() {
  static const SecretSchema schema = {
      "dev.poco.cyu-wifi-connect",
      SECRET_SCHEMA_NONE,
      {
          {"type", SECRET_SCHEMA_ATTRIBUTE_INTEGER},
      }};

  return &schema;
}
#define SCHEMA get_schema()

int get_pass(char *password) {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);

  int r = scanf("%s", password);

  term.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &term);

  return r;
}

const char *load_secret_part(enum TYPE type) {
  GError *error = NULL;
  const char *value =
      secret_password_lookup_sync(SCHEMA, NULL, &error, "type", type, NULL);
  if (error != NULL) {
    g_printerr("Failed to load secret: %s\n", error->message);
    g_error_free(error);
    exit(EXIT_FAILURE);
  }
  return value;
}

void store_secret_part(enum TYPE type, const char *value) {
  GError *error = NULL;
  secret_password_store_sync(SCHEMA, SECRET_COLLECTION_DEFAULT, "value", value,
                             NULL, &error, "type", type, NULL);
  if (error != NULL) {
    g_printerr("Failed to store secret: %s\n", error->message);
    g_error_free(error);
    exit(EXIT_FAILURE);
  }
}

env_t load_secret() {
  const char *username = load_secret_part(USERNAME);
  const char *password = load_secret_part(PASSWORD);
  return (env_t){username, password};
}

void store_secret(env_t env) {
  store_secret_part(USERNAME, env.username);
  store_secret_part(PASSWORD, env.password);
}

env_t ask_secret() {
  char username[256];
  char password[256];
  int rsc;
  do {
    printf("Username: ");
    rsc = scanf("%s", username);
  } while (rsc != 1);
  do {
    printf("Password: ");
    rsc = get_pass(password);
  } while (rsc != 1);
  return (env_t){username, password};
}
