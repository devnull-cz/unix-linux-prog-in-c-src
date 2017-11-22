/*
 * A very simple example on how a thread library implementation could be
 * achieved.  Remember that library threads can be used even on systems that
 * do not support threads in the kernel.  Basically, the user thread library
 * would change all blocking calls to nonblocking and if a call indicates that
 * it would block the process state is saved and one of the previously saved
 * states is used for a long jump.  Obviously, it is not trivial to write such a
 * library but this code gives you a general idea on how it could be done.
 *
 * The program saves the state and prints "0".  The first "if" is not satisfied
 * so we longjump() to the previously saved state.  "1" is then printed but
 * since "i" is 2 when the check is performed again, the program exists.  So, as
 * you can see, without any loop or a goto statement we print output twice using
 * just one fprintf() in the code.
 *
 * (c) jp@devnull.cz, vlada@devnull.cz
 */

#include <stdio.h>
#include <setjmp.h>

int
main(void)
{
	int i = 0, retval;
	jmp_buf env;

	/* Save the state. */
	retval = setjmp(env);
	fprintf(stderr, "%d [%d]\n", i, retval);

	++i;
	if (i == 2)
		return (1);

	longjmp(env, 4);

	return (0);
}
