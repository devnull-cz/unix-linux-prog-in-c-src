all: cstyle

.PHONY: cstyle
cstyle:
	@echo "checking C style"
	@find . -type f -name '*.c' | while read file; do \
		./cstyle.pl $$file; \
	done | grep ':'; \
	if [ $$? -eq 0 ]; then \
		exit 1; \
	fi
