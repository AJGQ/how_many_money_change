with import <nixpkgs> {}; {
    money-change-Env = gcc8Stdenv.mkDerivation {
        name = "money-change-env";
        buildInputs = [ stdenv
                        gcc
                        gdb
                        gmp
                      ];
    };
}
