{
  # libraries
  lib,
  mkShell,
  pkg-config,
  valgrind,

  # packages
  glib,
  networkmanager,
}:
let
  inherit (lib) makeIncludePath;
in
mkShell rec {
  nativeBuildInputs = [
    pkg-config
    valgrind
  ];
  buildInputs = [
    glib
    networkmanager
  ];
  # C_INCLUDE_PATH = makeIncludePath buildInputs;
  shellHook = ''
    export C_INCLUDE_PATH=$(pkg-config --cflags-only-I libnm | sed 's/-I//g' | tr ' ' :)
  '';
}
