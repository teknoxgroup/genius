Genius Calculator
=================

Although it's under heavy development, it's actually getting usable.
I use it myself as my desktop calculator.

To make myself look important, I also made up an official looking name for
the programming language of Genius, it's called GEL, (Genius Extention
Language) :)

Here's what doesn't work or isn't done yet: (somewhat of a TODO list)

- need to make more built-in functions!
- complex numbers (they mostly work now but need to be finished)
- CORBA interface (whichever GNOME will use)
- do a code clean-up (such as split up eval.[ch]) and make the names of
  types, functions and variables more consistent
- clean up calc.[ch] to be a better "interface to the calculator
- make the string functions stop reallocating ALL the time
- anonymous functions (functions that do not have a name, might be a
	problem though)
- profile and make the code leaner and meaner
- instead of functions calls and recursion, use an evaluation stack
- optimize the engine a bit more

Features of Genius:

 * arbitrary precision integers (2-36), multiple precision floats
 * uses rationals when possible
 * will calculate and show half calculated expressions if the calculation
   could not be completed
 * tries to optionally convert floats to integers to make calculatons more
   acurate (since integers are arbitrary precision)
 * can read prefix, infix or postfix expressions
 * variables
 * user functions
 * it will now add missing parenthesis on the ends of expressions
 * more ...

How to use this thing: (this is just a quick description)

Just type in the expression and press enter to get a result (or press the =
button). The expression is written in GEL (Genius Extention Language). 
A simple GEL expression looks just like a math expression. So you don't need
to learn GEL to use the calculator. GEL is used when you want to define 
and use functions, and variables.

****************************************************************************
What follows is a simple description of GEL:

A program in GEL is basically an expression which evaluates to a number,
since there exist only numbers.

For variables use the = operator, that operator sets the variable and
returns the number you set, so you can do something like "a=b=5", just
like in C. (this is in infix mode in postfix this would be "a b 5 = =").
Variables are really functions with no argument

there are a number of built in functions (currently "e" "pi" "sin" "cos"
"tan")

to type functions in:

infix)		function(argument1,argument1)
prefix)		function argument1 argument2 
postfix)	argument1 argument2 function 

(of course the number of arguments is different for each function, up to 8)

NOTE: that if there are no arguments you can't put any parenthesis around
them, so "function()" is illegal, it has to be "function", this is only
in infix mode.

To define a function do:

<number of arguments>\<function name> { <function body> }

to use the arguments use arg1, arg2 ... also note this is exactly the same
for all notations (prefix,infix,postfix) due to some limitations of the
parser I can't do this postfix-like for that notation

for example:

3\sum { arg1 + arg2 + arg3 }

then "sum(1,1,1)" yields 3

finally there is the ';' operator, which is a way to separate expressions,
such a combined expression will return whatever is the result of the last
one, so

3 ; 5

yeilds 5

this will require some parenthesizing to make it unambiguous sometimes,
especially if the ; is not the top most primitive

There are 3 constructs, if, ifelse and while. They are rather simple

here's the syntax:
infix or prefix)
	if expression1 (expression2)
	ifelse expression1 (expression2) (expression3)
	while expression1 (expression2)
postfix)
	expression1 (expression2) if
	expression1 (expression2) (expression3) ifelse
	expression1 (expression2) while

if will evaluate to expression2 if expression1 is not 0 otherwise it
will evaluate to 0.

ifelse will evaluate to expression2 if expression1 is not 0 otherwise it
will evaluate to expression3.

while will evaluate to the last iteration of expression2 or 0 if no
iterations were made. It will also evaluate expression2 n times and
expression1 n+1 times, where n is the number of iterations that were
done.

NOTE: Parenthesis are required around expressions 2 and 3 in both infix
and prefix, they are optional, but recommended in postfix.

And now the comparison operators:

==,>=,<=,!=,<,> return 1 for TRUE, 0 for FALSE

<=> returns -1 if left side is smaller, 0 if both sides are equal, 1
    if left side is larger

To build up logical expressions use the words "not","and","or","xor"


EXAMPLE PROGRAM in GEL:
    a user factorial function (there already is one built in so
    this function is useless)

    1\f { ifelse (arg1 <= 1) (1) (f(arg1-1) * arg1) }

    or with indentation it becomes

    1\f {
	    ifelse (arg1 <= 1)
		    (1)
		    (f(arg1-1) * arg1)
    }

    this is a direct port of the factorial function from the bc manpage :)

    here's an iterative version:

    1\f { r=arg1 ; while (arg1>1) ( arg1=arg1-1 ; r=r*arg1 ) ; r }

    let's try this in posfix:

    1\f { r arg1 = ; (arg1 1 >) (arg1 arg1 1 - =;r r arg1 * =) while;r }

    or fully parenthesized prefix (to make it lisplike if you wish)

    1\f { (= r arg1);while(> arg1 1)((= arg1 (- arg1 1));(= r (* r arg1)));r }
    

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
