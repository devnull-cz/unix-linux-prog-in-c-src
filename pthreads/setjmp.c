/*
 * A very simple example on how user thread library could be written. Remember
 * that library threads can be used even on systems that don't support threads
 * in the kernel. Basically, the user thread library would change all blocking
 * calls to nonblocking and if the call indicates that it would block then the
 * process state is saved and one of previously saved states is used for a
 * long jump. Obviously, it's not easy to write such a library but you get the
 * general idea on how to do it.
 *
 * The program saves the state and prints "1". The first "if" is not satisfied
 * so we longjump() to the saved state. Another "1" is printed but since "i"
 * is now 2, the program exists. So, as you can see, without any loop or goto
 * statement we print two 1's using just one fprintf() in the code.
 *
 * (c) jp@devnull.cz
 */

#include <stdio.h>
#include <setjmp.h>

int
main(void)
{
	int i = 0;
	jmp_buf env;

	setjmp(env);
	fprintf(stderr, "1\n");

	++i;
	if (i == 2)
		return (1);

	longjmp(env, 1);

	return (0);
}
