/*
   Practical
       So what is the point of select()?  Can't I just read and write to my  descriptors
       whenever  I  want?  The point of select() is that it watches multiple descriptors
       at the same time and properly puts the process to sleep if there is no  activity.
       UNIX  programmers  often  find themselves in a position where they have to handle
       I/O from more than one file descriptor where the data flow may  be  intermittent.
       If  you were to merely create a sequence of read(2) and write(2) calls, you would
       find that one of your calls may block waiting for data from/to a file descriptor,
       while  another  file  descriptor  is unused though ready for I/O.  select() effi‐
       ciently copes with this situation.

   Select law
       Many people who try to use select() come across behavior  that  is  difficult  to
       understand  and  produces  nonportable  or borderline results.  For instance, the
       above program is carefully written not to block at any point, even though it does
       not set its file descriptors to nonblocking mode.  It is easy to introduce subtle
       errors that will remove the advantage of using select(), so here  is  a  list  of
       essentials to watch for when using select().

       1.  You should always try to use select() without a timeout.  Your program should
           have nothing to do if there is no data available.  Code that depends on time‐
           outs is not usually portable and is difficult to debug.

       2.  The value nfds must be properly calculated for efficiency as explained above.

       3.  No file descriptor must be added to any set if you do not intend to check its
           result after the select() call, and respond appropriately.  See next rule.

       4.  After select() returns, all file descriptors in all sets should be checked to
           see if they are ready.

       5.  The  functions  read(2),  recv(2),  write(2),  and send(2) do not necessarily
           read/write the full amount of data that  you  have  requested.   If  they  do
           read/write  the  full  amount, it's because you have a low traffic load and a
           fast stream.  This is not always going to be the case.  You should cope  with
           the case of your functions managing to send or receive only a single byte.

       6.  Never  read/write  only  in single bytes at a time unless you are really sure
           that you have a small amount of data to process.  It is extremely inefficient
           not  to  read/write as much data as you can buffer each time.  The buffers in
           the example below are 1024 bytes although they could easily be made larger.

       7.  The functions read(2), recv(2), write(2), and send(2) as well as the select()
           call  can  return  -1  with  errno  set to EINTR, or with errno set to EAGAIN
           (EWOULDBLOCK).  These results must be properly  managed  (not  done  properly
           above).   If  your  program  is  not going to receive any signals, then it is
           unlikely you will get EINTR.  If your program does not set  nonblocking  I/O,
           you will not get EAGAIN.

       8.  Never  call  read(2),  recv(2),  write(2), or send(2) with a buffer length of
           zero.

       9.  If the functions read(2), recv(2), write(2), and  send(2)  fail  with  errors
           other than those listed in 7., or one of the input functions returns 0, indi‐
           cating end of file, then you should not  pass  that  descriptor  to  select()
           again.   In  the  example below, I close the descriptor immediately, and then
           set it to -1 to prevent it being included in a set.

       10. The timeout value must be initialized with each new call to  select(),  since
           some operating systems modify the structure.  pselect() however does not mod‐
           ify its timeout structure.

       11. Since select() modifies its file descriptor sets, if the call is  being  used
           in a loop, then the sets must be reinitialized before each call.

   Usleep emulation
       On  systems  that  do not have a usleep(3) function, you can call select() with a
       finite timeout and no file descriptors as follows:

           struct timeval tv;
           tv.tv_sec = 0;
           tv.tv_usec = 200000;
           select(0, NULL, NULL, NULL, &tv);

       This is guaranteed to work only on UNIX systems, however.

RETURN VALUE
       On success, select() returns the total number of file descriptors  still  present
       in the file descriptor sets.

       If  select() timed out, then the return value will be zero.  The file descriptors
       set should be all empty (but may not be on some systems).

       A return value of -1 indicates an error, with errno being set appropriately.   In
       the  case  of  an error, the contents of the returned sets and the struct timeout
       contents are undefined and should not be used.  pselect() however never  modifies
       ntimeout.

NOTES
       Generally  speaking,  all  operating  systems  that  support sockets also support
       select().  select() can be used to solve many problems in a  portable  and  effi‐
       cient  way that naive programmers try to solve in a more complicated manner using
       threads, forking, IPCs, signals, memory sharing, and so on.

       The poll(2) system call has the same functionality as select(), and  is  somewhat
       more  efficient  when  monitoring  sparse  file  descriptor sets.  It is nowadays
       widely available, but historically was less portable than select().

       The Linux-specific epoll(7) API provides an interface that is more efficient than
       select(2) and poll(2) when monitoring large numbers of file descriptors.

EXAMPLE
       Here  is  an  example that better demonstrates the true utility of select().  The
       listing below is a TCP forwarding program that forwards  from  one  TCP  port  to
       another.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

static int forward_port;

#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

static int
listen_socket(int listen_port)
{
   struct sockaddr_in a;
   int s;
   int yes;

   s = socket(AF_INET, SOCK_STREAM, 0);
   if (s == -1) {
       perror("socket");
       return -1;
   }
   yes = 1;
   if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
           &yes, sizeof(yes)) == -1) {
       perror("setsockopt");
       close(s);
       return -1;
   }
   memset(&a, 0, sizeof(a));
   a.sin_port = htons(listen_port);
   a.sin_family = AF_INET;
   if (bind(s, (struct sockaddr *) &a, sizeof(a)) == -1) {
       perror("bind");
       close(s);
       return -1;
   }
   printf("accepting connections on port %d\n", listen_port);
   listen(s, 10);
   return s;
}

static int
connect_socket(int connect_port, char *address)
{
   struct sockaddr_in a;
   int s;

   s = socket(AF_INET, SOCK_STREAM, 0);
   if (s == -1) {
       perror("socket");
       close(s);
       return -1;
   }

   memset(&a, 0, sizeof(a));
   a.sin_port = htons(connect_port);
   a.sin_family = AF_INET;

   if (!inet_aton(address, (struct in_addr *) &a.sin_addr.s_addr)) {
       perror("bad IP address format");
       close(s);
       return -1;
   }

   if (connect(s, (struct sockaddr *) &a, sizeof(a)) == -1) {
       perror("connect()");
       shutdown(s, SHUT_RDWR);
       close(s);
       return -1;
   }
   return s;
}

#define SHUT_FD1 do {                                \
                    if (fd1 >= 0) {                 \
                        shutdown(fd1, SHUT_RDWR);   \
                        close(fd1);                 \
                        fd1 = -1;                   \
                    }                               \
                } while (0)

#define SHUT_FD2 do {                                \
                    if (fd2 >= 0) {                 \
                        shutdown(fd2, SHUT_RDWR);   \
                        close(fd2);                 \
                        fd2 = -1;                   \
                    }                               \
                } while (0)

#define BUF_SIZE 1024

int
main(int argc, char *argv[])
{
   int h;
   int fd1 = -1, fd2 = -1;
   char buf1[BUF_SIZE], buf2[BUF_SIZE];
   int buf1_avail, buf1_written;
   int buf2_avail, buf2_written;

   if (argc != 4) {
       fprintf(stderr, "Usage\n\tfwd <listen-port> "
                "<forward-to-port> <forward-to-ip-address>\n");
       exit(EXIT_FAILURE);
   }

   signal(SIGPIPE, SIG_IGN);

   forward_port = atoi(argv[2]);

   h = listen_socket(atoi(argv[1]));
   if (h == -1)
       exit(EXIT_FAILURE);

   for (;;) {
       int r, nfds = 0;
       fd_set rd, wr, er;

       FD_ZERO(&rd);
       FD_ZERO(&wr);
       FD_ZERO(&er);
       FD_SET(h, &rd);
       nfds = max(nfds, h);
       if (fd1 > 0 && buf1_avail < BUF_SIZE) {
           FD_SET(fd1, &rd);
           nfds = max(nfds, fd1);
       }
       if (fd2 > 0 && buf2_avail < BUF_SIZE) {
           FD_SET(fd2, &rd);
           nfds = max(nfds, fd2);
       }
       if (fd1 > 0 && buf2_avail - buf2_written > 0) {
           FD_SET(fd1, &wr);
           nfds = max(nfds, fd1);
       }
       if (fd2 > 0 && buf1_avail - buf1_written > 0) {
           FD_SET(fd2, &wr);
           nfds = max(nfds, fd2);
       }
       if (fd1 > 0) {
           FD_SET(fd1, &er);
           nfds = max(nfds, fd1);
       }
       if (fd2 > 0) {
           FD_SET(fd2, &er);
           nfds = max(nfds, fd2);
       }

       r = select(nfds + 1, &rd, &wr, &er, NULL);

       if (r == -1 && errno == EINTR)
           continue;

       if (r == -1) {
           perror("select()");
           exit(EXIT_FAILURE);
       }

       if (FD_ISSET(h, &rd)) {
           unsigned int l;
           struct sockaddr_in client_address;

           memset(&client_address, 0, l = sizeof(client_address));
           r = accept(h, (struct sockaddr *) &client_address, &l);
           if (r == -1) {
               perror("accept()");
           } else {
               SHUT_FD1;
               SHUT_FD2;
               buf1_avail = buf1_written = 0;
               buf2_avail = buf2_written = 0;
               fd1 = r;
               fd2 = connect_socket(forward_port, argv[3]);
               if (fd2 == -1)
                   SHUT_FD1;
               else
                   printf("connect from %s\n",
                           inet_ntoa(client_address.sin_addr));
           }
       }

       /* NB: read oob data before normal reads */

       if (fd1 > 0)
           if (FD_ISSET(fd1, &er)) {
               char c;

               r = recv(fd1, &c, 1, MSG_OOB);
               if (r < 1)
                   SHUT_FD1;
               else
                   send(fd2, &c, 1, MSG_OOB);
           }
       if (fd2 > 0)
           if (FD_ISSET(fd2, &er)) {
               char c;

               r = recv(fd2, &c, 1, MSG_OOB);
               if (r < 1)
                   SHUT_FD2;
               else
                   send(fd1, &c, 1, MSG_OOB);
           }
       if (fd1 > 0)
           if (FD_ISSET(fd1, &rd)) {
               r = read(fd1, buf1 + buf1_avail,
                         BUF_SIZE - buf1_avail);
               if (r < 1)
                   SHUT_FD1;
               else
                   buf1_avail += r;
           }
       if (fd2 > 0)
           if (FD_ISSET(fd2, &rd)) {
               r = read(fd2, buf2 + buf2_avail,
                         BUF_SIZE - buf2_avail);
               if (r < 1)
                   SHUT_FD2;
               else
                   buf2_avail += r;
           }
       if (fd1 > 0)
           if (FD_ISSET(fd1, &wr)) {
               r = write(fd1, buf2 + buf2_written,
                          buf2_avail - buf2_written);
               if (r < 1)
                   SHUT_FD1;
               else
                   buf2_written += r;
           }
       if (fd2 > 0)
           if (FD_ISSET(fd2, &wr)) {
               r = write(fd2, buf1 + buf1_written,
                          buf1_avail - buf1_written);
               if (r < 1)
                   SHUT_FD2;
               else
                   buf1_written += r;
           }

       /* check if write data has caught read data */

       if (buf1_written == buf1_avail)
           buf1_written = buf1_avail = 0;
       if (buf2_written == buf2_avail)
           buf2_written = buf2_avail = 0;

       /* one side has closed the connection, keep
          writing to the other side until empty */

       if (fd1 < 0 && buf1_avail - buf1_written == 0)
           SHUT_FD2;
       if (fd2 < 0 && buf2_avail - buf2_written == 0)
           SHUT_FD1;
   }//infinite for loop
   exit(EXIT_SUCCESS);
}