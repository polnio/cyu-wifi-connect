{
  # Libraries
  stdenv,
  pkg-config,

  # Dependencies
  curl,
  glib,
  libsecret,
  networkmanager,
}:
stdenv.mkDerivation {
  pname = "cyu-wifi-connect";
  version = "0.1.0";
  src = ../.;
  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    curl
    glib
    libsecret
    networkmanager
  ];
  buildPhase = ''
    echo $PKG_CONFIG_PATH
    $CC src/main.c -o cyu-wifi-connect $(pkg-config --cflags --libs libnm libcurl libsecret-1)
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp cyu-wifi-connect $out/bin
  '';
}
