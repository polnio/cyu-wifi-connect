{
  # libraries
  callPackage,
  mkShell,
  valgrind,
}:
let
  package = callPackage ./package.nix { };
in
mkShell ({
  nativeBuildInputs = [
    valgrind
  ];
  inputsFrom = [ package ];
  # C_INCLUDE_PATH = makeIncludePath buildInputs;
  shellHook = ''
    export C_INCLUDE_PATH=$(pkg-config --cflags-only-I libnm libcurl libsecret-1 | sed 's/-I//g' | tr ' ' :)
    export CPLUS_INCLUDE_PATH=$C_INCLUDE_PATH
  '';
})
