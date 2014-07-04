REPL
====

Operations:

exit
add [int] [int] -> [int]: Addition.
sub [int] [int] -> [int]: Subtraction.
mul [int] [int] -> [int]: Multiplication.
div [int] [int] -> [int] || [error]: Division.
rem [int] [int] -> [int] || [error]: Remainder.
neg [int] -> [int]: Negation.
and [boolean] [boolean] -> [boolean]: Logical AND.
or [boolean] [boolean] -> [boolean]: Logical OR.
not [boolean] -> [boolean]: Logical negation.
if [element] [element] [boolean] -> [element]: Returns first element if true, else returns second element.
lessThan [int] [int] -> [boolean]: Relational less than.
equal [int] [int] -> [boolean]: Relational equal to.
bind [element] [name]: Binds a value to a name.
load [string] -> [boolean]: Loads the contents of a filename and runs the code within that file.
apply [closure]: Executes code in closure.
pop: Removes top item from stack.
exc: Exchanges top two items on stack.
quit: Exits program.

types:

int: Any integer.
name: Alphanumeric string that starts with a letter.
string: Anything between two quotation marks ("string").
closure: Anything between an opening and closing bracket ({closure}).
error literal: ':error:'.
boolean literals: ':false:', ':true:'.
