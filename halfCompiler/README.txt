Author: Michael Shaffer

Executable: ./pda

Files Tested: p1.pas , p2.pas , e1.pas , e2.pas , e3.pas , e4.pas

Description: This program functions as the first two components of a compiler; the scanner and parser, which scans a stream of chars and parses
them into tokens. If certain formats are not followed, error messages are thrown.

Known Bugs: Adding the "no production found" message into my program caused the program to throw that error constantly. It seemed like it was
checking the wrong value. I commented it out of my program but as this error should never occur in the grammar provided, this did 
not affect my program. All other error messages worked. 
I had to set the first token ID as "program". I tried alternate methods to hardcoding it but these did not work. 

