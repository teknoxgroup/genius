Genius Calculator
=================

**********************************************************
(Read Changes-0.2 for changes description from 0.1 to 0.2)
Read NEWS for new stuff
**********************************************************

Although it's under heavy development, it's actually getting usable.
I use it myself as my desktop calculator.

To make myself look important, I also made up an official looking name for
the programming language of Genius, it's called GEL, (Genius Extention
Language) :)

Here's what doesn't work or isn't done yet: (somewhat of a TODO list)

- need to make more built-in functions!
- complex numbers (they mostly work now but need to be finished)
- CORBA interface
- do a code clean-up (such as split up eval.[ch]) and make the names of
  types, functions and variables more consistent
- clean up calc.[ch] to be a better "interface to the calculator
- make the string functions stop reallocating ALL the time
- profile and make the code leaner and meaner
- optimize the engine a bit more

Features of Genius:

 * arbitrary precision integers (2-36), multiple precision floats
 * uses rationals when possible
 * will calculate and show half calculated expressions if the calculation
   could not be completed
 * tries to optionally convert floats to integers to make calculatons more
   acurate (since integers are arbitrary precision)
 * variables
 * user functions
 * variable and function references with C like syntax
 * anonymous functions
 * it will now add missing parenthesis on the ends of expressions (only in
   the GUI version)
 * more ...

How to use this thing: (this is just a quick description)

Just type in the expression and press enter to get a result (or press the =
button). The expression is written in GEL (Genius Extention Language). 
A simple GEL expression looks just like a math expression. So you don't need
to learn GEL to use the calculator. GEL is used when you want to define 
and use functions, and variables.

The program reads two files as gel programs as startup
<prefix>/share/genius/lib.gel and ~/.gnomeinit, lib.gel has some standard
functions of the language implemented in gel itself

****************************************************************************
What follows is a simple description of GEL:

A program in GEL is basically an expression which evaluates to a number,
since there exist only numbers.

For variables use the = operator, that operator sets the variable and
returns the number you set, so you can do something like "a=b=5", just
like in C. Variables are really functions with no argument

There are a number of built in functions (currently "e" "pi" "sin" "cos"
"tan", and a number of other (check funclib.c, all the way on the bottom))

To type functions in:

function(argument1, argument2, argument3)
function()
function

(of course the number of arguments is different for each function)

NOTE: that if there are no arguments you can but don't have to put
the parentheses in

To define a function do:

define <identifier>(<comma separated argument names>) { <function body> }

or alternatively

<identifier> = `(<comma separated argument names>) { <function body> }

NOTE: that's a backquote and signifies an anonymous function, by setting
it to a variable name you effectively define a function

for example:

define sum(a,b,c) { a+b+c}

then "sum(1,1,1)" yields 3

finally there is the ';' operator, which is a way to separate expressions,
such a combined expression will return whatever is the result of the last
one, so

3 ; 5

yeilds 5

this will require some parenthesizing to make it unambiguous sometimes,
especially if the ; is not the top most primitive

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
	- gtk+
	- gnome libs
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
