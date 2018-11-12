# voglib - Voice Of God
Interface for (hackish) communication between a testing utility and Pic MCU via mdb

The project is named such because of how this library will facilitate communication. It will work by directly reading from and writing to the memory of a microcontroller (simulated or otherwise) via the Microchip debugger (mdb) that ships with MPLAB X IDE.

Currently, this projecct relies on mdblib which can be found here: https://github.com/ctl106/mdblib
I currently lack a makefile to make building with it easier, so you will have to forgive me.
When building a project with this and mdblib, you will need to make sure the headers are included in the search path for your compiler (via writing your own makefile for your project, or otherwise) AND make sure that the .c files are included as source files in the call to the compiler.

More information will come as development progresses, and there will inevitably be examples of how to use this library as it will be a dependancy of xctest, another project of mine.
