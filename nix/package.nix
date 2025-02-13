{
  stdenv,
}:
stdenv.mkDerivation {
  pname = "cyu-wifi-connect";
  version = "0.1.0";
  src = ../.;
  buildPhase = ''
    $CC src/main.c -o cyu-wifi-connect
  '';
  installPhase = ''
    mkdir -p $out/bin
    cp cyu-wifi-connect $out/bin
  '';
}
