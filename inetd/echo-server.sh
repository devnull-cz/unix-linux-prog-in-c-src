#!/bin/sh
#
# A simple echo server for use with inetd(8). The server must be used with a
# streamed socket and the "nowait" flag. As you see, we don't call accept(2)
# so we can't use it with the "wait" flag. So, for example, if you put this
# line into your inetd.conf on FreeBSD box (usually inetd.conf syntax is the
# same on other systems as well but not sure if krb524 is defined in
# /etc/services anywhere):
#
# krb524  stream  tcp     nowait  root /path/to/echo-server.sh echo-server.sh
#
# and you connect to localhost (or any other interface with the port not
# protected by a firewall) on port 4444, what you send there is sent back and
# the server exits. When tried from the terminal, what is sent is determined
# by the newline which is when nc(1) or telnet(1) return from read(2). You can
# run the test in parallel since inetd spanws a new server and is immediatelly
# ready to accept other connections. Example:
#
# $ nc -v localhost 4444
# Connection to localhost 4444 port [tcp/krb524] succeeded!
# hello there!
# hello there!
# $
#
# (c) jp@devnull.cz
#

read a
echo $a

exit 0
