
{pkgs}: {
  deps = [
    pkgs.tk
    pkgs.tcl
    pkgs.qhull
    pkgs.pkg-config
    pkgs.gtk3
    pkgs.gobject-introspection
    pkgs.ghostscript
    pkgs.freetype
    pkgs.ffmpeg-full
    pkgs.cairo
    pkgs.glibcLocales
    pkgs.postgresql
    pkgs.gcc
    pkgs.cmake
    pkgs.python311
    pkgs.python311Packages.matplotlib
    pkgs.python311Packages.pandas
  ];
}
