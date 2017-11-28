MAKE=make

SUBDIRS=assert		\
	debug		\
	pipe		\
	err		\
	dyn-lib		\
	dead-lock	\
	cond-variables	\
	basic-utils	\
	exec		\
	exit		\
	file-locking	\
	fork		\
	getopt		\
	inetd		\
	lib-abi		\
	main		\
	malloc		\
	mmap		\
	mutexes		\
	pmap		\
	pthreads	\
	race		\
	read		\
	readdir		\
	reentrant	\
	resolving	\
	select		\
	semaphores	\
	session		\
	setuid		\
	signals		\
	sigsegv		\
	sleep		\
	stat		\
	tcp		\
	udp		\
	uid		\
	unix-socket	\
	unix-standards	\
	wait

all: $(SUBDIRS) cstyle

$(SUBDIRS):
	@$(MAKE) -C $@

clean cstyle:
	ret=0; \
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
		if [ $$? -ne 0 ]; then \
			ret=1; \
		fi \
	done; \
	exit $$ret

.PHONY: cstyle all clean $(SUBDIRS)
