# Caravel 4ft4

4ft4 is a clone of the MCS-4 architecture (4004 CPU, 4001 ROM, 4002 RAM),
with a wishbone backdoor interface to the "ROM" and the RAM that enables
loading a program for execution and inspecting/modifying the RAM.

This is an integration of 4ft4 into the caravel harness, with the 4ft4 wishbone
backdoor interface connected to the caravel management processor.

### License information

caravel_user_project and modifications: Apache 2.0. The original
caravel_user_project that this is based on is
https://github.com/efabless/caravel_user_project.

4ft4: BSD.
