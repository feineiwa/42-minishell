PERROR(3)                                                                                Linux Programmer's Manual                                                                               PERROR(3)

NNAAMMEE
       perror - print a system error message

SSYYNNOOPPSSIISS
       ##iinncclluuddee <<ssttddiioo..hh>>

       vvooiidd ppeerrrroorr((ccoonnsstt cchhaarr **_s));;

       ##iinncclluuddee <<eerrrrnnoo..hh>>

       ccoonnsstt cchhaarr ** ccoonnsstt _s_y_s___e_r_r_l_i_s_t[[]];;
       iinntt _s_y_s___n_e_r_r;;
       iinntt _e_r_r_n_o;;       /* Not really declared this way; see errno(3) */

   Feature Test Macro Requirements for glibc (see ffeeaattuurree__tteesstt__mmaaccrrooss(7)):

       _s_y_s___e_r_r_l_i_s_t, _s_y_s___n_e_r_r:
           From glibc 2.19 to 2.31:
               _DEFAULT_SOURCE
           Glibc 2.19 and earlier:
               _BSD_SOURCE

DDEESSCCRRIIPPTTIIOONN
       The ppeerrrroorr() function produces a message on standard error describing the last error encountered during a call to a system or library function.

       First  (if  _s  is not NULL and _*_s is not a null byte ('\0')), the argument string _s is printed, followed by a colon and a blank.  Then an error message corresponding to the current value of _e_r_r_n_o
       and a new-line.

       To be of most use, the argument string should include the name of the function that incurred the error.

       The global error list _s_y_s___e_r_r_l_i_s_t[], which can be indexed by _e_r_r_n_o, can be used to obtain the error message without the newline.  The largest message number provided in the table  is  _s_y_s___n_e_r_r-1.
       Be careful when directly accessing this list, because new error values may not have been added to _s_y_s___e_r_r_l_i_s_t[].  The use of _s_y_s___e_r_r_l_i_s_t[] is nowadays deprecated; use ssttrreerrrroorr(3) instead.

       When  a system call fails, it usually returns -1 and sets the variable _e_r_r_n_o to a value describing what went wrong.  (These values can be found in _<_e_r_r_n_o_._h_>.)  Many library functions do likewise.
       The function ppeerrrroorr() serves to translate this error code into human-readable form.  Note that _e_r_r_n_o is undefined after a successful system call or library  function  call:  this  call  may  well
       change  this  variable,  even though it succeeds, for example because it internally used some other library function that failed.  Thus, if a failing call is not immediately followed by a call to
       ppeerrrroorr(), the value of _e_r_r_n_o should be saved.

VVEERRSSIIOONNSS
       Since glibc version 2.32, the declarations of _s_y_s___e_r_r_l_i_s_t and _s_y_s___n_e_r_r are no longer exposed by _<_s_t_d_i_o_._h_>.

AATTTTRRIIBBUUTTEESS
       For an explanation of the terms used in this section, see aattttrriibbuutteess(7).

       ┌──────────┬───────────────┬─────────────────────┐
       │IInntteerrffaaccee │ AAttttrriibbuuttee     │ VVaalluuee               │
       ├──────────┼───────────────┼─────────────────────┤
       │ppeerrrroorr()  │ Thread safety │ MT-Safe race:stderr │
       └──────────┴───────────────┴─────────────────────┘

CCOONNFFOORRMMIINNGG TTOO
       ppeerrrroorr(), _e_r_r_n_o: POSIX.1-2001, POSIX.1-2008, C89, C99, 4.3BSD.

       The externals _s_y_s___n_e_r_r and _s_y_s___e_r_r_l_i_s_t derive from BSD, but are not specified in POSIX.1.

NNOOTTEESS
       The externals _s_y_s___n_e_r_r and _s_y_s___e_r_r_l_i_s_t are defined by glibc, but in _<_s_t_d_i_o_._h_>.

SSEEEE AALLSSOO
       eerrrr(3), eerrrrnnoo(3), eerrrroorr(3), ssttrreerrrroorr(3)

CCOOLLOOPPHHOONN
       This page is part of release 5.10 of the Linux _m_a_n_-_p_a_g_e_s project.  A description of the project, information about reporting  bugs,  and  the  latest  version  of  this  page,  can  be  found  at
       https://www.kernel.org/doc/man-pages/.

                                                                                                2020-11-01                                                                                       PERROR(3)
