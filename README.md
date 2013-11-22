Genius Calculator
=================

Although it's under heavy development, it's actually getting usable.
I use it myself as my desktop calculator.

To make myself look important, I also made up an official looking name for
the programming language of Genius, it's called GEL, (Genius Extention
Language) :)

Here's what doesn't work or isn't done yet: (somewhat of a TODO list)

- need to make more built-in functions!
- complete complex number and matrix support, right now neither is complete
- CORBA interface
- do a code clean-up (such as split up eval.[ch]) and make the names of
  types, functions and variables more consistent
- clean up calc.[ch] to be a better "interface to the calculator"
- profile and make the code leaner and meaner
- optimize the engine a bit more
- find all the annoying memory leaks

Features of Genius:

 * arbitrary precision integers (2-36), multiple precision floats
 * uses rationals when possible
 * will calculate and show half calculated expressions if the calculation
   could not be completed
 * variables
 * user functions
 * variable and function references with C like syntax
 * anonymous functions
 * it will add missing parenthesis on the ends of expressions (only in
   the GUI version)
 * matrix support
 * complex numbers
 * more ...

How to use this thing: (this is just a quick description)

Just type in the expression and press enter to get a result (or press the =
button). The expression is written in GEL (Genius Extention Language). 
A simple GEL expression looks just like a math expression. So you don't need
to learn GEL to use the calculator. GEL is used when you want to define 
and use functions, and variables.

The command line version is actually nicer to use I think and in the future
I'll work on the GUI a bit more to make it as good as the command line
version. To call up the command line version just execute "genius". I myself
don't use the GUI version because of it's shortcomings. Most likely the next
version of the GUI will be the comman dline version running in a terminal
widget with some GUI stuff added to it.

****************************************************************************
What follows is a simple description of GEL:

A program in GEL is basically an expression which evaluates to a number,
since there exist only numbers.

Previous result:

The "Ans" variable can be used to get the result of the last expression

so if you did some calculation and just wanted to add 389 to the result,
you'd do "Ans+389"

Functions:

For variables use the = operator, that operator sets the variable and
returns the number you set, so you can do something like "a=b=5", just
like in C. Variables are really functions with no argument

There are a number of built in functions (currently "e" "pi" "sin" "cos"
"tan", and more)

To type functions in:

function(argument1, argument2, argument3)
function()
function

(of course the number of arguments is different for each function)

NOTE: that if there are no arguments you can but don't have to put
the parentheses in.

Current built in functions:

name:		# of arguments:		description:

warranty	0			prints warranty and license info
exit		0			exits genius
quit		0			exits genius
error		1 (string)		prints an error to the error channel
print		1 (string)		prints a string onto the standard
					output
printn		1 (string)		prints a string onto the standard
					output without trailing carrige
					return
display		2 (string,value)	prints a string, a colon and then the
					value

sin		1 (value)		calculates the sin function
cos		1 (value)		calculates the cos function
tan		1 (value)		calculates the tan function
pi		0 			the number pi
e		0 			the number e (same as exp(1), but
					answer is cached)
sqrt		1 (value)		calculates the sqare root
exp		1 (value)		calculates the e^value (exponential
					function)
ln		1 (value)		calculates the natural logarithm
gcd		2 (integer,integer)	the greatest common divisor function
lcm		2 (integer,integer)	the least common multiple function
max		2 (value,value)		returns the larger of the two values
min		2 (value,value)		returns the smaller of the two values
prime		1 (integer)		returns the n'th prime for primes up
					to n==100000
round		1 (value)		rounds the value
floor		1 (value)		greatest integer <= value
ceil		1 (value)		least integer >= value
trunc		1 (value)		truncate the number to an integer
Re		1 (value)		return the real part of a complex number
Im		1 (value)		return the imaginary part of a complex number
I		1 (integer)		return an identity matrix of the size n

is_value	1 			returns true if the argument is a
					scalar value
is_string	1 			returns true if the argument is a
					string
is_matrix	1 			returns true if the argument is a
					matrix
is_complex	1 			returns true if the argument is a
					complex number
is_integer	1 			returns true if the argument is an
					integer
is_rational	1 			returns true if the argument is an
					rational, note that it is false if
					it's an integer!
is_float	1			returns true if the argument is a
					floating point number
is_null		1			returns true if the argument is a
					null (empty result not a 0)

Functions in the standard library (lib.gel):

name:		# of arguments:		description:

sum		3 (from,to,function)	calculates the sum of the function
					which should take one argument
					which is an integer stepped from
					"from" to "to
prod		3 (from,to,function)	calculates the product of the
					function which should take one
					argument which is an integer
					stepped from "from" to "to
nPr		2 (integer,integer)	calculate permutations
nCr		2 (integer,integer)	calculate combinations
fib		1 (integer)		returns n'th fibbonachi number


To define a function do:

define <identifier>(<comma separated argument names>) { <function body> }

or alternatively

<identifier> = `(<comma separated argument names>) { <function body> }

NOTE: that's a backquote and signifies an anonymous function, by setting
it to a variable name you effectively define a function

for example:

define sum(a,b,c) { a+b+c}

then "sum(1,1,1)" yields 3

Absoulte value:

You can make an absolute value of something by putting the |'s around it.
Example:

|a-b|

Separator:

Finally there is the ';' operator, which is a way to separate expressions,
such a combined expression will return whatever is the result of the last
one, so

3 ; 5

yeilds 5

This will require some parenthesizing to make it unambiguous sometimes,
especially if the ; is not the top most primitive. This slightly differs
from other programming languages where the ; is a terminator of statements,
whereas in GEL it's actually a binary operator.

The GEL operators:

a;b		separator, just evaluates both but returns only b
a=b		assignment operator asigns b to a (a must be a valid lvalue)
|a|		absolute value
a^b		exponentiation
a+b		addition
a-b		subtraction
a*b		multiplication
a/b		division
a%b		the mod operator
a!		factorial operator
a==b		equality operator (returns 1 or 0)
a!=b		inequality operator (returns 1 or 0)
a<=b		inequality operator (returns 1 or 0)
a>=b		inequality operator (returns 1 or 0)
a<=>b		comparison operator (returns -1, 0 or 1)
a and b		logical and
a or b		logical or
a xor b		logical xor
not a		logical not
-a		negation operator
&a		variable referencing (to pass a reference to something)
*a		variable dereferencing (to access a referenced varible)
a'		matrix transpose
a@(b,c)		get element of a matrix
a@(b,)		get row of a matrix
a@(,c)		get column of a matrix

There are also a number of constructs:

Conditionals:

if <expression1> then <expression2> [else <expression3>]

If else is omitted, then if the expression1 yeilds 0, NULL is returned.

Examples:

if(a==5)then(a=a-1)
if b<a then b=a
if c>0 then c=c-1 else c=0
a = ( if b>0 then b else 1 )

Loops:

while <expression1> do <expression2>
until <expression1> do <expression2>
do <expression2> while <expression1>
do <expression2> until <expression1>

These are similiar to other languages, they return the result of
the last iteration or NULL if no iteration was done

And now the comparison operators:

==,>=,<=,!=,<,> return 1 for TRUE, 0 for FALSE

<=> returns -1 if left side is smaller, 0 if both sides are equal, 1
    if left side is larger

To build up logical expressions use the words "not","and","or","xor"

Null:

Null is a special value, if it is returned, nothing is printed on
screen, no operations can be done on it. It is also usefull if you want
no output from a command. Null can be achieved as an expression when you
type . or nothing

Example:

    x=5;.
    x=5;

Returning:

Sometimes it's not good to return the last thing calculated, you may for
example want to return from a middle of a function. This is what the return
keyword is for, it takes one argument which is the return value

Example:

    define f(x) {
	    y=1;
	    while(1) do (
		    if(x>50) then return y;
		    y=y+1;
		    x=x+1;
	    );
    }	

References: 

GEL contains references with a C like syntax. & references a variable
and * dereferences a variable, both can only be applied to an identifier
so **a is not legal in GEL

Example:

    a=1;
    b=&a;
    *b=2;

    now a contains 2

    define f(x){x+1};
    t=&f;
    *t(3)

    gives us 4

Anonymous functions:

It is possible to say use a function in another function yet you don't know
what the function will be, you use an anonymous function. Anonymous function
is declared as:

`(<comma separated argument names>) { <function body> }

NOTE: Unlike in setting a variable, when an anonymous function is passed it
is passed as reference, this is for consistency for function using functional
arguments

Example:

    define f(a,b) {*a(b)+1};
    f(`(x){x*x},2)

    will return 5 (2*2+1)

You can also use function references for that:

    define f(a,b) {*a(b)+1};
    define b(x){x*x};
    f(&b,2)

Matrix entry:

To enter matrixes use one of the following two syntaxes. You can either
enter the matrix separating values by commas and rows by semicolons, or
separating values by tabs and rows by returns, or any combination of the
two. So to enter a 3x3 matrix of numbers 1-9 you could do

[1,2,3:4,5,6:7,8,9]

or

[1	2	3
 4	5	6
 7	8	9]

or

[1,2,3
 4,5,6
 7,8,9]

Do not use both ':' and return at once on the same line though. You can
however use tabs and commas together, as long as you use at most 1 comma
to separate values. To enter tabs inside the command line version, you have
to do M-Tab (usually Alt-Tab), or however else you have your .inputrc set
up (since it uses readline)

You can also use the matrix expansion functionality to enter matricies.
For example you can do:
a = [	1	2	3
	4	5	6
	7	8	9]
b = [	a	10
	11	12]

and you should get

[1	2	3	10
 4	5	6	10
 7	8	9	10
 11	11	11	12]

similiarly you can build matricies out of vectors and other stuff like that.

Another thing is that non-specified spots are initialized to 0, so

[1	2	3
 4	5
 6]

will end up being

[1	2	3
 4	5	0
 6	0	0]

NOTE: be careful about using whitespace and returns for expressions inside
the '[',']' brackets, they have a slightly different meaning and could mess
you up.

Transpose operator:

You can transpose a matrix by using the ' operator, example:

[1,2,3]*[4,5,6]'

We transpose the second vector to make matrix multiplication possible.

Error handeling:

If you detect an error in your function, you can bail out of it. You can
either fail to compute the function which is for normal errors, such as
wrong types of arguments, just add the empty statement "bailout". If something
went really wrong and you want to completely kill the current computation,
you can use "exception".

Look at lib.gel for some examples.


EXAMPLE PROGRAM in GEL:
    a user factorial function (there already is one built in so
    this function is useless)

    define f(x) { if x<=1 then 1 else (f(x-1)*x) }

    or with indentation it becomes

    define f(x) {
	    if x<=1 then
		    1
	    else
		    (f(x-1)*x)
    }

    this is a direct port of the factorial function from the bc manpage :)
    it seems similiar to bc syntax, but different in that in gel, the
    last expression is the one that is returned. It can be done with with
    returns as:

    define f(x) {
	    if (x <= 1) then return (1);
	    return (f(x-1) * x);
    }

    which is almost verbatim bc code, except for then

    here's an iterative version:

    define f(x) {
	    r=x;
	    while (x>1) do
		    (x=x-1;r=r*x);
	    r
    }

****************************************************************************

NOTE: GMP sefaults on some very large numbers so factorials of large numbers
will do this ... I'll try to catch those before they happen, but I don't
see this as a high priority ... the listbox will segfault anyway on such
numbers :)

Requirements:
	- lex (tested under flex)
	- yacc (tested under bison -y)
	- gmp (tested with 2.0.2)
	- glib
	- gtk+ (only for the GUI version)
	- gnome libs (only for the GUI version)
All except gmp seem to be pretty much standard or Linux systems, and even
on most other platforms. (except gtk and gnome libs, but since this is
distributed with gnome ...)

It's under GPL so read COPYING

George <jirka@5z.com>

-----------------------------------------------------------------------------
OLD history, read ChangeLog for new changes:

what's new since pre-alpha-1:
	- support for all the primitives
	- some scientific functions
	- uses gnomeapp
	- variables
	- user defined functions
	- and a lot more ... read the ChangeLog!

what's new since unusable-3:
	- version change :)
	- can read arbitrary ints (base#number#, e.g. 7#442#)
	- again some very slight tinkering with the gui
	- exponentials using rationals are now calculated via Newton's
	  method (e.g. 3^(33/12) )

what's new since unusable-2:
	- integer exponantiation, meaning you can do <whatever>^<integer>
	- the display box now stretches out to make use of free space in
	  the window
	- characters from the numpad now properly insert themselves
	- a bit of code cleanup
	- automaic conversion of floats to integers can be turned off since
	  it can make less precise numbers look precise
	- you can also turn off exact answers and get answers to a certain
	  precision (10 digits), or get the whole enchilada (sometimes
	  the listbox will crash for very large integers)

what's new since unusable-1:
	- floats are now formatted and output ok.
	- options frame for swiching between notations
	- negative numbers are handled by lexer so that they work in
	  postfix and prefix, this broke parser for infix, so turn off
	  negative functions for infix and use the unary minus operator
	  instead (same result)
	- factorial added (only for ints :)
	- it will now convert to int whenever possible even inside
	  calculations
	- parser.c renamed to calc.c since it didn't makes sense
	- GUI changed a bit
	- negation function is now functional it is ~ now ... but -
	  can also be used in infix, but not in postfix or prefix
	  since that would be ambiguous though you can still write
	  a negative number in any notation
	- division now works, it will make a rational number from integers
	  but will do a division for floats or rationals (rationals make
	  precise divisions)
	- it can now display unevaluated parts of equation when an error
	  occurs or a part just can't be evaluated (such as division by
	  zero)
	- results are now put on the display list in the GUI.
