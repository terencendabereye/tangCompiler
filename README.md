## **Tang Compiler for Custom Microprocessor**


### Introduction
The purpose of this project is to create a compiler based on a custom languate I created called _t-lang_ or _tang_. It compiles into a machine code for a microprocessor (called [GPC-3][toSystem3]) that i created in [Logisim][toLogisim] and run within that simulator.
This project is part of my goal to understand exactly how computers work, primarly all the levels of abstraction, from single transistors to simple software(like simple calculators and embedded systems).

### Project Spectifications
This project was developed and tested on macOS Monterey using the following software;
* Clang, C compiler
* Flex, Fast lexical analyser generator
* Bison, Parser generator

Not that the compiler is written primarily in the C language (not Flex & Bison which are written in their specific languages).

This project is divided into three parts. Parts separate parts;
1. [Assembler](assembler)  
This receives assembly and converts it into machine code in a format that can be fed into the simulator. 
2. [Label Replacer](labelReplacement)  
This component takes assembly containing labels and label calls (references to labels); removes the labels and replaces them with appropritate line numbers. This component is primarily used in fo fix jump instructions. When jumping ahead in the program, the compiler doesn't know the address of the target location, so labels are used. Additionally, the system uses 16-bit addresses but 8-bit words (8-bit registers). Therefore an address has to be split into two 8-bit integers. This is also handled by the label replacer.  
Labels are writen in the following format;  
`<lbl>` where `lbl` can be any text. This label stores the 16-bit line number where it is located. It is converted to an address before it is stored.  
`<lbl,0>` the number after the comma (`0` here) is the least significant half (or lower 8-bits) of the address where the label was first used. `<lbl,0>` is the most signinficant half. (or upper 8-bits).
3. [Compiler](compiler)  
This is the main part of the program and contains code to combine the assembler and label replacer with the compiler code.  
It contains the language analysis code like the parse, lexer and related functions, as well as the user interface code.

### T-language Specification
This section defines the rules of the T-language.  

Input files must have `.tang` extension. This is not enforced in code but is required if you use the [vs-tang][tang-compiler-support] vsCode extension.

This section will be completed at a later date. In its place her is some sample code.

```
byteout('>');
.start;
    var x;
    var y;
    var a;

    .getx();
    byteout('+');
    .gety();
    a=(x-y);
    byteout('=');
    byteout(0x30+a);
    byteout(0xa);
    byteout('>');
    
    
    goto(.start);

.getx;
    x = bytein();
    if (x==0) .getx;
    byteout(x);
    x = x-0x30;
    return;
.gety;
    y = bytein();
    byteout(y);
    if (y==0) .getx;
    y = y-0x30;
    return;
```

### Using the Compiler
The tool can be used with the command `./tang` when the [exectuable](tang) is in the current working directory.  
  
One can try out the code from the command line on any Unix based system (like Linux, macOS etc.). I have not included the executable with the source, therefore we build it.
Note you must have Flex, Bison, Make and a C compiler (must support c99).
```
mkdir t-lang
cd t-lang
git clone https://github.com/terencendabereye/tangCompiler.git
cd tangCompiler
make build
```
To test the executable, I have included a `.tang` source file. Run the following code
```
./tang -s tang.tang -o out.txt
```
Since Logisim doesn't require a special file extension, the output can just be a `.txt` file.  
You can then load this `out.txt` file into the rom of the Logisim simulation.

[toLogisim]: http://www.cburch.com/logisim/
[toSystem3]: https://github.com/terencendabereye/microprocessor
[tang-compiler-support]: https://github.com/terencendabereye/vs-tang
