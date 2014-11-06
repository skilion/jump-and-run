Jump & Run
==========

Old game made in high school

##### NOTE: this game requires [OpenAL](http://www.openal.org/)

## Compilation on Windows

1. Build the Visual Studio solution in the External directory
2. Build the Visual Studio solution in the Source directory

## Compilation on Linux

1. `cd Intermediate`
2. `cmake ../External -DCMAKE_BUILD_TYPE=Release; make`
3. `rm CMakeCache.txt`
4. `cmake ../Source -DCMAKE_BUILD_TYPE=Release; make`

## Music used:

* from the album "Audionautix: Tech, Urban, Dance" by Jason Shaw
  * "The Big House"

* from the album "Straw Fields" by Rolemusic
  * "Yellow Dust"
  * "Leafless Quince Tree"

* from the album "Utter Mess" by Edward Shallow
  * "You've Only Yourself To Blame For Your Tragic Loss In Faith"
  * "We Need Cranes, Not Skyhooks"
  * "Inside (Reprise)"
