# interpreter


<cut />
<br clear = "left">

---
To download, enter in the command line: 
```
git clone https://github.com/Ollasni/interpreter
```
To compile, enter: 
```
make
```
### This interpreter can:
 * Read code;
 * Divide the code into tokens;
 * Build an infix and a postfix;
 * Work with arithmetic, assignment, logical operators;
 * Work with Variables;
 * Work of the special jump operator, named "goto";
 * Work with a `while` cycle, conditional statements("if", "else");
 * Work with arrays;



**Interpreter consists of nain stages:**
1. Read the source code of the program, create an infix(parseLexem function);
2. Initialize all labels and goto's positions;
3. Convert infix to postfix(buildPoliz);
5. Calculations are performed (evaluatePoliz) .

**Examples**

1. simple arithmetic operations
	'2 + 9'
	'6 / 2'
2. variables
	'x := 3
	x + 1'
3. goto labels:
	'1 + 2
	goto L
	3 - 5
	L:
	1 + 2'
4. conditions:
	'x := 1
	while x > 5 then
	x++
	endwhile
	1 + 3'
5. arrays:
	'a array 5
	a[4] := 3
	a[4] - 1'

