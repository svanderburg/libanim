{ nixpkgs ? <nixpkgs>
, systems ? [ "i686-linux" "x86_64-linux" ]
, libanim ? {outPath = ./.; rev = 1234;}
, libiffJobset ? import ../libiff/release.nix { inherit nixpkgs systems officialRelease; }
, libilbmJobset ? import ../libilbm/release.nix { inherit nixpkgs systems officialRelease; }
, officialRelease ? false
}:

let
  pkgs = import nixpkgs {};

  version = builtins.readFile ./version;

  jobs = rec {
    tarball =
      with pkgs;

      let
        libiff = builtins.getAttr (builtins.currentSystem) (libiffJobset.build);
        libilbm = builtins.getAttr (builtins.currentSystem) (libilbmJobset.build);
      in
      releaseTools.sourceTarball {
        name = "libanim-tarball";
        src = libanim;
        inherit version officialRelease;
        dontBuild = false;
        CFLAGS = "-ansi -pedantic -Wall";

        buildInputs = [ pkgconfig help2man libiff libilbm ];
      };

    build =
      pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };

        let
          libiff = builtins.getAttr system (libiffJobset.build);
          libilbm = builtins.getAttr system (libilbmJobset.build);
        in
        releaseTools.nixBuild {
          name = "libanim";
          inherit version;
          src = tarball;
          buildInputs = [ pkgconfig libiff libilbm ];
          CFLAGS = "-ansi -pedantic -Wall";
        }
      );
  };
in
jobs
