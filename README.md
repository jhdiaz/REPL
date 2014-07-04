REPL
====

Implemented a stack-based Read-Evaluate-Print-Loop that was able to handle many different data types (such as Boolean literals, strings, numbers, closures, and names), and perform many different operations (such as arithmetic operations, logical operations, relational operations, a bind operation that associated a value with a name type, and a load operation that used the contents of a file as the input). This stack-based REPL would push an evaluated item onto the stack and use the top n (n = number of arguments required for a command) elements of the stack as the arguments for a command, providing that the arguments were of the correct type.  CSE 305, Introduction to Programming Languages, Spring 2014
