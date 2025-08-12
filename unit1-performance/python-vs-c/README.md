# Demo: Why do people think higher-level languages are slow?

## Part 1: add two numbers in C

Check out `add.c`. Let's compile it to assembly only, without creating a binary. 

```
gcc -O0 -S add.c
```

This creates `add.s`. Open this file in a text editor and look for code for the `add()` function. How many machine instructions is this function?

I see several instructions that manipulate the stack pointer, a `ret`, and five instructions that are actually doing the work of the function. (If we turned on optimizations, the compiler would inline this function and remove the stack pointer work and the `ret`.) Of those five instructions, four are loading the inputs `a` and `b`, and the fifth as and `add` instruction.

## Part 2: add two numbers in Python

Now let's do the same thing in Python. Open up `add.py` to see the program. It's the Python equivalent of our C program.Next, disassemble the Python program:

```
python3 disassemble_python.py
```

Notice that the Python program is doing the same thing! It reads `a` and `b` from memory, calls `add`, and returns. But this is hiding something big! The lines of this disassembly are lines of *Python Bytecode*, not machine instructions. To see what that means, keep reading.

## Part 3: Open up a bytecode instruction

We'll take one of the operations in the Python disassembly: `BINARY_OP (+)`. It takes some digging, but we can find the machine instructions behind this operation.

```
objdump -d /usr/bin/python3 | grep "<PyNumber_Add@@Base>:" -A 1000 > PyNumber_Add_assembly.txt
```

This disassembles the entire Python binary with `objdump -d`. Then it searches for the start of the instructions for the binary add function. It dumps 1000 lines starting at the beginning of the instructions for that function, and saves it to a file called `PyNumber_Add_assembly.txt`. 

Open up `PyNumber_Add_assembly.txt`. How many instructions long is this function?

## Conclusion

A high level language like Python is doing a lot more work than C. Simply adding two numbers can involve thousands of lines of machine instructions. (Due to branching, perhaps only hundreds of these lines run on a given execution of this ByteCode instruction.) But in C, the same operation is only about 5 instructions. The essential reason that Python needs to do so much more work is that C is stongly typed, but in Python a variable can be any type, and those types can change at run time.

