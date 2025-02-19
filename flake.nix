{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-parts = {
      url = "github:hercules-ci/flake-parts";
      inputs.nixpkgs-lib.follows = "nixpkgs";
    };
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];
      perSystem =
        { pkgs, ... }:
        {
          packages = rec {
            cyu-wifi-connect = pkgs.callPackage ./nix/package.nix { };
            default = cyu-wifi-connect;
          };
          devShells.default = pkgs.callPackage ./nix/shell.nix { };
        };
    };
}
