{pkgs}: {
  deps = [
    pkgs.postgresql
    pkgs.gcc
    pkgs.cmake
    pkgs.imagemagick  # เพิ่ม ImageMagick
  ];
}
