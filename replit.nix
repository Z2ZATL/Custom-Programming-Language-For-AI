
{pkgs}: {
  deps = [
    pkgs.postgresql
    pkgs.gcc
    pkgs.cmake
    pkgs.python311
    pkgs.python311Packages.matplotlib
    pkgs.python311Packages.pandas
  ];
}
