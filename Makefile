MAKE=make

SUBDIRS=assert		\
	debug		\
	pipe		\
	err		\
	dyn-lib		\
	debug		\
	dead-lock	\
	cond-variables	\
	basic-utils	\
	exec		\
	exit		\
	file-locking	\
	fork		\
	getopt

all: $(SUBDIRS) cstyle

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done

.PHONY: cstyle all clean $(SUBDIRS)
cstyle:
	@echo "checking C style"
	@find . -type f -name '*.c' | while read file; do \
		./cstyle.pl $$file; \
	done | grep ':'; \
	if [ $$? -eq 0 ]; then \
		exit 1; \
	fi
