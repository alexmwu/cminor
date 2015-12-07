<div class="header">

# Overview of C-Minor 2015

</div>
This md file was generated with [to-markdown](https://domchristie.github.io/to-markdown/).

This is an informal specification of C-Minor 2015, a C-like language for use in an undergraduate compilers class. C-minor includes expressions, basic control flow, recursive functions, and strict type checking. It is object-code compatible with ordinary C and thus can take advantage of the standard C library, within its defined types. It is similar enough to C to feel familiar, but different enough to give you some sense of alternatives.

This document is deliberately informal: the most precise specification of a language is the compiler itself, and it is your job to write the compiler! It is your job to read the document carefully and extract a formal specification. You will certainly find elements that are unclear or incompletely specified, and you are encouraged to raise questions in class.

## Whitespace and Comments

In C-minor, whitespace is any combination of the following characters: tabs, spaces, linefeed (\n), and carriage return (\r). The placement of whitespace is not significant in C-minor. Both C-style and C++-style comments are valid in C-minor:

<pre>/* A C-style comment */
a=5; // A C++ style comment
</pre>

## Identifiers

Identifiers (i.e. variable and function names) may contain letters, numbers, and underscores. Identifiers must begin with a letter or an underscore. These are examples of valid identifiers:

<pre>i x mystr fog123 BigLongName55
</pre>

The following strings are C-minor keywords and may not be used as identifiers:

<pre>array boolean char else false for function if integer print return string true void while
</pre>

## Types

C-minor has four atomic types: integers, booleans, characters, and strings. A variable is declared as a name followed by a colon, then a type and an optional initializer. For example:

<pre>x: integer;
y: integer = 123;
b: boolean = false;
c: char    = 'q';
s: string  = "hello cminor\n";
</pre>

An **integer** is always a signed 64 bit value. **boolean** can take the literal values **true** or **false**. **char** is a single 8-bit ASCII character. **string** is a double-quoted constant string that is null-terminated and cannot be modified.

Both **char** and **string** may contain the following backslash codes. **\n** indicates a linefeed (ASCII value 10), **\0** indicates a null (ASCII value zero), and a backslash followed by anything else indicates exactly the following character. Both strings and identifiers may be up to 256 characters long.

C-minor also supports arrays of a fixed size. They may be declared with no value, which causes them to contain all zeros:

<pre>a: array [5] integer;
</pre>

Or, the entire array may be given specific values:

<pre>a: array [5] integer = {1,2,3,4,5};
</pre>

## Expressions

C-minor has many of the arithmetic operators found in C, with the same meaning and level of precedence:

<table cellborder="1">

<tbody>

<tr>

<td>() [] f()</td>

<td>grouping, array subscript, function call</td>

</tr>

<tr>

<td>++ --</td>

<td>postfix increment, decrement</td>

</tr>

<tr>

<td>- **!**</td>

<td>unary negation, **logical not (note change)**</td>

</tr>

<tr>

<td>^</td>

<td>exponentiation</td>

</tr>

<tr>

<td>* / %</td>

<td>multiplication, division, modulus</td>

</tr>

<tr>

<td>+ -</td>

<td>addition, subtraction</td>

</tr>

<tr>

<td>< <= > >= == !=</td>

<td>comparison</td>

</tr>

<tr>

<td>&&</td>

<td>logical and</td>

</tr>

<tr>

<td>||</td>

<td>logical or</td>

</tr>

<tr>

<td>=</td>

<td>assignment</td>

</tr>

</tbody>

</table>

C-minor is **strictly typed**. This means that you may only assign a value to a variable (or function parameter) if the types match **exactly**. You cannot perform many of the fast-and-loose conversions found in C.

Following are examples of some (but not all) type errors:

<pre>x: integer = 65;
y: char = 'A';
if(x>y) ... // error: x and y are of different types!

f: integer = 0;
if(f) ...      // error: f is not a boolean!

writechar: function void ( char c );
a: integer = 65;
writechar(a);  // error: a is not a char!

b: array [2] boolean = {true,false};
x: integer = 0;
x = b[0];      // error: x is not a boolean!
</pre>

Following are some (but not all) examples of correct type assignments:

<pre>b: boolean;
x: integer = 3;
y: integer = 5;
b = x<y;     // ok: the expression x<y is boolean

f: integer = 0;
if(f==0) ...    // ok: f==0 is a boolean expression

c: char = 'a';
if(c=='a') ...  // ok: c and 'a' are both chars
</pre>

## Declarations and Statements

In C-minor, you may declare global variables with optional constant initializers, function prototypes, and function definitions. Within functions, you may declare local variables (including arrays) with optional initialization expressions. Scoping rules are identical to C. Function definitions may not be nested.

Within functions, basic statements may be arithmetic expressions, return statements, print statements, if and if-else statements, for loops, or code within inner { } groups. C-minor does not have switch statements, while-loops, or do-while loops.

The <tt>print</tt> statement is a little unusual because it is a statement and not a function call. <tt>print</tt> takes a list of expressions separated by commas, and prints each out to the console, like this:

<pre>print "The temperature is: ", temp, " degrees\n";
</pre>

## Functions

Functions are declared in the same way as variables, except giving a type of <tt>function</tt> followed by the return type, arguments, and code:

<pre>square: function integer ( x: integer ) = {
  return x^2;
}
</pre>

The return type must be one of the four atomic types, or <tt>void</tt> to indicate no type. Function arguments may be of any type. <tt>integer</tt>, <tt>boolean</tt>, and <tt>char</tt> arguments are passed by value, while <tt>string</tt> and <tt>array</tt> arguments are passed by reference. As in C, arrays passed by reference have an indeterminate size, and so the length is typically passed as an extra argument:

<pre>printarray: function void ( a: array [] integer, size: integer ) = {
  i: integer;
  for( i=0;i<size;i++) {
    print a[i], "\n";
  }
}

</pre>

A function prototype may be given, which states the existence and type of the function, but includes no code. This must be done if the user wishes to call an external function linked by another library. For example, to invoke the C function <tt>puts</tt>:

<pre>puts: function void ( s: string );

main: function integer () = {
  puts("hello world");
}
</pre>

A complete program must have a <tt>main</tt> function that returns an integer. the arguments to <tt>main</tt> may either be empty, or use <tt>argc</tt> and <tt>argv</tt> as in C. (The declaration of <tt>argc</tt> and <tt>argv</tt> is left as an exercise to the reader.)
