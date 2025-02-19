{
  stdenv,
  pkg-config,
  networkmanager,
  glib,
}:
stdenv.mkDerivation {
  pname = "cyu-wifi-connect";
  version = "0.1.0";
  src = ../.;
  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    glib
    networkmanager
  ];
  buildPhase = ''
    echo $PKG_CONFIG_PATH
    $CC src/main.c -o cyu-wifi-connect $(pkg-config --cflags --libs libnm)
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp cyu-wifi-connect $out/bin
  '';
}
