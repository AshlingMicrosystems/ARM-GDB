/* Replay a remote debug session logfile for GDB.
   Copyright (C) 1996-2025 Free Software Foundation, Inc.
   Written by Fred Fish (fnf@cygnus.com) from pieces of gdbserver.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#undef PACKAGE
#undef PACKAGE_NAME
#undef PACKAGE_VERSION
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME

#include <config.h>
#include "gdbsupport/version.h"

#if HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#if HAVE_SIGNAL_H
#include <signal.h>
#endif
#include <ctype.h>
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <unistd.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#if HAVE_NETDB_H
#include <netdb.h>
#endif
#if HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif

#if USE_WIN32API
#include <ws2tcpip.h>
#endif

#include "gdbsupport/netstuff.h"
#include "gdbsupport/rsp-low.h"

#include "getopt.h"

#ifndef HAVE_SOCKLEN_T
typedef int socklen_t;
#endif

/* Sort of a hack... */
#define EOL (EOF - 1)

static int remote_desc_in;
static int remote_desc_out;
/* When true all packets are printed to stderr as they are handled by
   gdbreplay.  */
bool debug_logging = false;

static void
sync_error (FILE *fp, const char *desc, int expect, int got)
{
  fprintf (stderr, "\n%s\n", desc);
  fprintf (stderr, "At logfile offset %ld, expected '0x%x' got '0x%x'\n",
	   ftell (fp), expect, got);
  fflush (stderr);
  exit (1);
}

static void
remote_error (const char *desc)
{
  fprintf (stderr, "\n%s\n", desc);
  fflush (stderr);
  exit (1);
}

static void
remote_close (void)
{
#ifdef USE_WIN32API
  gdb_assert (remote_desc_in == remote_desc_out);
  closesocket (remote_desc_in);
#else
  close (remote_desc_in);
  if (remote_desc_in != remote_desc_out)
    close (remote_desc_out);
#endif
}

/* Open a connection to a remote debugger.
   NAME is the filename used for communication.  */

static void
remote_open (const char *name)
{
#ifndef USE_WIN32API
  if (strcmp (name, "-") == 0)
    {
      remote_desc_in = 0;
      remote_desc_out = 1;
      return;
    }
#endif

  const char *last_colon = strrchr (name, ':');

  if (last_colon == NULL)
    {
      fprintf (stderr, "%s: Must specify tcp connection as host:addr\n", name);
      fflush (stderr);
      exit (1);
    }

#ifdef USE_WIN32API
  static int winsock_initialized;
#endif
  int tmp;
  int tmp_desc;
  struct addrinfo hint;
  struct addrinfo *ainfo;

  memset (&hint, 0, sizeof (hint));
  /* Assume no prefix will be passed, therefore we should use
     AF_UNSPEC.  */
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = IPPROTO_TCP;

  parsed_connection_spec parsed = parse_connection_spec (name, &hint);

  if (parsed.port_str.empty ())
    error (_("Missing port on hostname '%s'"), name);

#ifdef USE_WIN32API
  if (!winsock_initialized)
    {
      WSADATA wsad;

      WSAStartup (MAKEWORD (1, 0), &wsad);
      winsock_initialized = 1;
    }
#endif

  int r = getaddrinfo (parsed.host_str.c_str (), parsed.port_str.c_str (),
		       &hint, &ainfo);

  if (r != 0)
    {
      fprintf (stderr, "%s:%s: cannot resolve name: %s\n",
	       parsed.host_str.c_str (), parsed.port_str.c_str (),
	       gai_strerror (r));
      fflush (stderr);
      exit (1);
    }

  scoped_free_addrinfo free_ainfo (ainfo);

  struct addrinfo *p;

  for (p = ainfo; p != NULL; p = p->ai_next)
    {
      tmp_desc = socket (p->ai_family, p->ai_socktype, p->ai_protocol);

      if (tmp_desc >= 0)
	break;
    }

  if (p == NULL)
    perror_with_name ("Cannot open socket");

  /* Allow rapid reuse of this port. */
  tmp = 1;
  setsockopt (tmp_desc, SOL_SOCKET, SO_REUSEADDR, (char *) &tmp,
	      sizeof (tmp));

  switch (p->ai_family)
    {
    case AF_INET:
      ((struct sockaddr_in *) p->ai_addr)->sin_addr.s_addr = INADDR_ANY;
      break;
    case AF_INET6:
      ((struct sockaddr_in6 *) p->ai_addr)->sin6_addr = in6addr_any;
      break;
    default:
      fprintf (stderr, "Invalid 'ai_family' %d\n", p->ai_family);
      exit (1);
    }

  if (bind (tmp_desc, p->ai_addr, p->ai_addrlen) != 0)
    perror_with_name ("Can't bind address");

  fprintf (stderr, "Replay logfile using %s\n", name);
  fflush (stderr);
  if (p->ai_socktype == SOCK_DGRAM)
    remote_desc_in = tmp_desc;
  else
    {
      struct sockaddr_storage sockaddr;
      socklen_t sockaddrsize = sizeof (sockaddr);
      char orig_host[GDB_NI_MAX_ADDR], orig_port[GDB_NI_MAX_PORT];

      if (listen (tmp_desc, 1) != 0)
	perror_with_name ("Can't listen on socket");

      remote_desc_in = accept (tmp_desc, (struct sockaddr *) &sockaddr,
			       &sockaddrsize);

      if (remote_desc_in == -1)
	perror_with_name ("Accept failed");

      /* Enable TCP keep alive process. */
      tmp = 1;
      setsockopt (tmp_desc, SOL_SOCKET, SO_KEEPALIVE,
		  (char *) &tmp, sizeof (tmp));

      /* Tell TCP not to delay small packets.  This greatly speeds up
	 interactive response. */
      tmp = 1;
      setsockopt (remote_desc_in, IPPROTO_TCP, TCP_NODELAY,
		  (char *) &tmp, sizeof (tmp));

      if (getnameinfo ((struct sockaddr *) &sockaddr, sockaddrsize,
		       orig_host, sizeof (orig_host),
		       orig_port, sizeof (orig_port),
		       NI_NUMERICHOST | NI_NUMERICSERV) == 0)
	{
	  fprintf (stderr, "Remote debugging from host %s, port %s\n",
		   orig_host, orig_port);
	  fflush (stderr);
	}

#ifndef USE_WIN32API
      close (tmp_desc);		/* No longer need this */

      signal (SIGPIPE, SIG_IGN);	/* If we don't do this, then
					   gdbreplay simply exits when
					   the remote side dies.  */
#else
      closesocket (tmp_desc);	/* No longer need this */
#endif
    }

#if defined(F_SETFL) && defined (FASYNC)
  fcntl (remote_desc_in, F_SETFL, FASYNC);
#endif
  remote_desc_out = remote_desc_in;
}

static int
logchar (FILE *fp, bool print)
{
  int ch;
  int ch2;

  ch = fgetc (fp);
  if (ch != '\r' && (print || debug_logging))
    {
      fputc (ch, stderr);
      fflush (stderr);
    }
  switch (ch)
    {
      /* Treat \r\n as a newline.  */
    case '\r':
      ch = fgetc (fp);
      if (ch == '\n')
	ch = EOL;
      else
	{
	  ungetc (ch, fp);
	  ch = '\r';
	}
      if (print || debug_logging)
	{
	  fputc (ch == EOL ? '\n' : '\r', stderr);
	  fflush (stderr);
	}
      break;
    case '\n':
      ch = EOL;
      break;
    case '\\':
      ch = fgetc (fp);
      if (print || debug_logging)
	{
	  fputc (ch, stderr);
	  fflush (stderr);
	}
      switch (ch)
	{
	case '\\':
	  break;
	case 'b':
	  ch = '\b';
	  break;
	case 'f':
	  ch = '\f';
	  break;
	case 'n':
	  ch = '\n';
	  break;
	case 'r':
	  ch = '\r';
	  break;
	case 't':
	  ch = '\t';
	  break;
	case 'v':
	  ch = '\v';
	  break;
	case 'x':
	  ch2 = fgetc (fp);
	  if (print || debug_logging)
	    {
	      fputc (ch2, stderr);
	      fflush (stderr);
	    }
	  ch = fromhex (ch2) << 4;
	  ch2 = fgetc (fp);
	  if (print || debug_logging)
	    {
	      fputc (ch2, stderr);
	      fflush (stderr);
	    }
	  ch |= fromhex (ch2);
	  break;
	case 'c':
	  fputc (ch, stderr);
	  fflush (stderr);
	  break;
	case 'E':
	  fputc (ch, stderr);
	  fflush (stderr);
	  break;
	default:
	  /* Treat any other char as just itself */
	  break;
	}
    default:
      break;
    }
  return (ch);
}

static int
gdbchar (int desc)
{
  unsigned char fromgdb;

  if (read (desc, &fromgdb, 1) != 1)
    return -1;
  else
    return fromgdb;
}

/* Accept input from gdb and match with chars from fp (after skipping one
   blank) up until a \n is read from fp (which is not matched) */

static void
expect (FILE *fp)
{
  int fromlog;
  int fromgdb;

  if ((fromlog = logchar (fp, false)) != ' ')
    {
      sync_error (fp, "Sync error during gdb read of leading blank", ' ',
		  fromlog);
    }
  do
    {
      fromlog = logchar (fp, false);
      if (fromlog == EOL)
	break;
      fromgdb = gdbchar (remote_desc_in);
      if (fromgdb < 0)
	remote_error ("Error during read from gdb");
    }
  while (fromlog == fromgdb);

  if (fromlog != EOL)
    {
      sync_error (fp, "Sync error during read of gdb packet from log", fromlog,
		  fromgdb);
    }
}

/* Calculate checksum for the packet stored in buffer buf.  Store
   the checksum in a hexadecimal format in a checksum_hex variable.  */
static void
recalculate_csum (const std::string &buf, int off, unsigned char *checksum_hex)
{
  unsigned char csum = 0;

  int len = buf.length ();
  for (int i = off; i < len; ++i)
    csum += buf[i];

  checksum_hex[0] = tohex ((csum >> 4) & 0xf);
  checksum_hex[1] = tohex (csum & 0xf);
}

/* Play data back to gdb from fp (after skipping leading blank) up until a
   \n is read from fp (which is discarded and not sent to gdb). */

static void
play (FILE *fp)
{
  int fromlog;
  int where_csum = 0, offset = 1;
  unsigned char checksum[2] = {0, 0};
  std::string line;


  if ((fromlog = logchar (fp, false)) != ' ')
    {
      sync_error (fp, "Sync error skipping blank during write to gdb", ' ',
		  fromlog);
    }
  while ((fromlog = logchar (fp, false)) != EOL)
    {
      if (fromlog == '#')
	where_csum = line.length ();
      line.push_back (fromlog);
    }

  /* Packet starts with '+$' or '$', we don't want to calculate those
     to the checksum, subtract the offset to adjust the line length.
     If the line starts with '$', the offset remains set to 1.  */
  if (line[0] == '+')
    offset = 2;

  if (where_csum > 0)
    line.resize (where_csum);
  recalculate_csum (line, offset, checksum);

  line.push_back ('#');
  line.push_back (checksum[0]);
  line.push_back (checksum[1]);

  if (write (remote_desc_out, line.data (), line.size ()) != line.size ())
    remote_error ("Error during write to gdb");
}

static void
gdbreplay_version (void)
{
  printf ("GNU gdbreplay %s%s\n"
	  "Copyright (C) 2024 Free Software Foundation, Inc.\n"
	  "gdbreplay is free software, covered by "
	  "the GNU General Public License.\n"
	  "This gdbreplay was configured as \"%s\"\n",
	  PKGVERSION, version, host_name);
}

static void
gdbreplay_usage (FILE *stream)
{
  fprintf (stream, "Usage:\tgdbreplay LOGFILE HOST:PORT\n");
}

static void
gdbreplay_help ()
{
  gdbreplay_usage (stdout);

  printf ("\n");
  printf ("LOGFILE is a file generated by 'set remotelogfile' in gdb.\n");
  printf ("COMM may either be a tty device (for serial debugging),\n");
  printf ("HOST:PORT to listen for a TCP connection, or '-' or 'stdio' to use\n");
  printf ("stdin/stdout of gdbserver.\n");
  printf ("\n");

  printf ("Options:\n\n");
  printf ("  --debug-logging       Show packets as they are processed.\n");
  printf ("  --help                Print this message and then exit.\n");
  printf ("  --version             Display version information and then exit.\n");
  if (REPORT_BUGS_TO[0])
    {
      printf ("\n");
      printf ("Report bugs to \"%s\".\n", REPORT_BUGS_TO);
    }
}

/* Main function.  This is called by the real "main" function,
   wrapped in a TRY_CATCH that handles any uncaught exceptions.  */

[[noreturn]] static void
captured_main (int argc, char *argv[])
{
  FILE *fp;
  int ch, optc;
  enum opts { OPT_VERSION = 1, OPT_HELP, OPT_LOGGING };
  static struct option longopts[] =
    {
	{"version", no_argument, nullptr, OPT_VERSION},
	{"help", no_argument, nullptr, OPT_HELP},
	{"debug-logging", no_argument, nullptr, OPT_LOGGING},
	{nullptr, no_argument, nullptr, 0}
    };

  while ((optc = getopt_long (argc, argv, "", longopts, nullptr)) != -1)
    {
      switch (optc)
	{
	case OPT_VERSION:
	  gdbreplay_version ();
	  exit (0);
	case OPT_HELP:
	  gdbreplay_help ();
	  exit (0);
	case OPT_LOGGING:
	  debug_logging = true;
	  break;

	case '?':
	  fprintf (stderr,
		   "Use 'gdbreplay --help' for a complete list of options.\n");
	  exit (1);
	}
    }

  if (optind + 2 != argc)
    {
      gdbreplay_usage (stderr);
      exit (1);
    }
  fp = fopen (argv[optind], "r");
  if (fp == NULL)
    {
      perror_with_name (argv[optind]);
    }
  remote_open (argv[optind + 1]);
  while ((ch = logchar (fp, false)) != EOF)
    {
      switch (ch)
	{
	case 'w':
	  /* data sent from gdb to gdbreplay, accept and match it */
	  expect (fp);
	  break;
	case 'r':
	  /* data sent from gdbreplay to gdb, play it */
	  play (fp);
	  break;
	case 'c':
	  /* We want to always print the command executed by GDB.  */
	  if (!debug_logging)
	    {
	      fprintf (stderr, "\n");
	      fprintf (stderr, "Command expected from GDB:\n");
	    }
	  while ((ch = logchar (fp, true)) != EOL);
	  break;
	case 'E':
	  if (!debug_logging)
	    fprintf (stderr, "E");
	  while ((ch = logchar (fp, true)) != EOL);
	  break;
	}
    }
  remote_close ();
  exit (0);
}

int
main (int argc, char *argv[])
{
  try
    {
      captured_main (argc, argv);
    }
  catch (const gdb_exception &exception)
    {
      if (exception.reason == RETURN_ERROR)
	{
	  fflush (stdout);
	  fprintf (stderr, "%s\n", exception.what ());
	}

      exit (1);
    }

  gdb_assert_not_reached ("captured_main should never return");
}
