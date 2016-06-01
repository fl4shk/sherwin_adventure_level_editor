sherwin\_adventure\_level\_editor
=================================

This isn't complete yet, but it functions.


Dependencies for Everywhere
---------------------------
1.  GNU Make (That's right... POSIX Make isn't enough!)
2.  GCC (with G++) version 4.9 (probably) or higher, due to the use of
-std=c++14.
4.  Qt5
5.  SFML (Only 2.x versions supported)
6.  pugixml
7.  gbalzss (included in devkitARM) (Probably __only needed for level
exporting__ once that has been completely implemented)



Non-Windows Dependencies
------------------------
1.  pkg-config (unless you want to change the makefile)



Instructions for Setting up Pugixml on Windows
----------------------------------------------
1.  Grab the latest pugixml archive (probably the one with Windows Line
Endings) from [here](http://pugixml.org/).
2.  From the archive, extract the files from the __src__ directory in the
archive and put them in the __pugixml_source__ directory.
3.  At this point, run the __make__ command.  


An Extra Tip
------------
1.  It's probably a good idea to use the -j argument to the __make__
command to speed up compliation.


Definite Goals
--------------
1.  Add in level exporting.  This is __definitely__ needed.


Possible Goals
--------------
1.  Remove the dependency on SMFL.  This is a possible goal because SFML
doesn't seem to work on my single board computers and other stuff that I
would like for this program to work on.

