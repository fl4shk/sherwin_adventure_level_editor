sherwin\_adventure\_level\_editor
=================================

This isn't complete yet, but it functions.


Dependencies
------------
1.  GNU Make (That's right... POSIX Make isn't enough!)
2.  GCC (with G++) version 4.9 (probably) or higher, due to the use of
-std=c++14.
2.  pkg-config (unless you want to change the makefile)
3.  Qt5
4.  SFML (Only 2.x versions supported)
5.  pugixml
6.  gbalzss (included in devkitARM) (Probably only needed for level
exporting once that has been implemented)


Possible Goals
--------------
1.  Remove the dependency on SMFL.  This is a possible goal because SFML
doesn't seem to work on my single board computers, which I would like to
support.


Definite Goals
--------------
1.  Add in level exporting.  This is __definitely__ needed.

