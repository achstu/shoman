{pkgs ? import <nixpkgs> {}}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
  ];
  shellHook = ''
    alias ll="ls -lah"
  '';
}
