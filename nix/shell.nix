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
    export C_INCLUDE_PATH=$(pkg-config --cflags-only-I libnm | sed 's/-I//g' | tr ' ' :)
    if [ -f .env ]; then
      export $(cat .env | xargs)
    fi
  '';
})
