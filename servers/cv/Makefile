# Makefile for IPC server
PROG=	cv	
SRCS=	main.c condition_variables.c lock.c signals.c table.c utility.c

DPADD+=	${LIBSYS}
LDADD+=	-lsys

MAN=

BINDIR?= /usr/sbin
FILES=cv.conf
FILESNAME=cv
FILESDIR= /etc/system.conf.d

.include <minix.service.mk>
