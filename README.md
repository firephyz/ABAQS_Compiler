# ABAQS_Compiler

This compiler targets the ABAQS architecture developed by Alan Ehret, Peter Jamieson and Michel A. Kinsy. Source files are SBML cell models (with possible to-be-determined extensions).

* Requires libSBML-5.17.0-beta - Installing and linking this library proved to be quite difficult. Make sure your headers get installed in /usr/include and
your libs get installed in /usr/lib. All standard locations.
  * We require the beta version since it adds partial support for the spatial package.

* Built with GCC-7 - Assuming your compiler supports c++17, you can change lines 11 and 12 in $TOP_DIR/configure to your compiler paths.

Information on the ABAQS architecture can be found here: http://ascslab.org/research/abc/abaqs/index.html
