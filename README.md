# FlightSimulator

This is a study project of Erel Shtossel for first degree in CS for Bar-Ilan University.

The software is a kind of a compiler for a language that can control a plane in the flight simulator "Flight Gear".

In the language you can, for now:
- Control the engine, wings and controls of the plane.
- Get information from the plane and its current state.

To use this software:
1) Download [Flight Gear](https://www.flightgear.org/download/).
2) Download\Clone the code.
3) Put the `generic_small.xml` in the FlightGear protocol folder (Ex: /usr/share/games/flightgear).
4) Add this configurations to FlightGear in "additional settings": `--telnet=socket,in,10,127.0.0.1,5402,tcp --httpd=8080 --generic=socket,out,10,127.0.0.1,5400,tcp,generic_small`. (change the ports 5402, 8080, 5400 as you wish). 
5) Compile the project with:  `g++ -std=c++11 *.cpp calc/*.cpp` or run it with an IDE.
6) Run the program with your commands in a text file as a parameter (Ex: `./a.out ../fly.txt`).
7) Run FlightGear.

Here is an example of a valid command text file that starts the engine, does a liftoff and gets to 1000 altitude: [fly](/fly.txt)

Notice:
The language that this software can compile, for now:
- Print to console with the command: `Print(_text with "" or a varible_)`.
- Pause the run with the command: `Pause(_time_)`.
- Open a server for Flightgear to connect to with the command: `openDataServer(_port_)`.
- Open a client to connect to FlightGear with the command: `connectControlClient(_port_)`.
- Define a variable in mode read(`<-`) or write(`->`) to the simulator with the command: `var name mode sim("_generic_path.xml path_")`.
- Define a normal variable that holds a double in this syntax: `name_of_var = value` or a simple calculation with numbers or other variables.
- Handle `if` and `while` commands in this syntax: 
```
while\if condition {
commands
}
```
For more information please contact me.
