{
  # Libraries
  stdenv,
  pkg-config,

  # Dependencies
  networkmanager,
  glib,
  curl,
}:
stdenv.mkDerivation {
  pname = "cyu-wifi-connect";
  version = "0.1.0";
  src = ../.;
  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    glib
    networkmanager
    curl
  ];
  buildPhase = ''
    echo $PKG_CONFIG_PATH
    $CC src/main.c -o cyu-wifi-connect $(pkg-config --cflags --libs libnm libcurl)
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp cyu-wifi-connect $out/bin
  '';
}
