<h1> shash </h1>

<h3> Introduction </h3>

Presenting shash's shell - a Linux styled shell written in C  <br>

<h3> How to use </h3>

<ol>
    <li> <code>cd</code> Into the directory </li>
    <li> Run the <code>make</code> command </li>
    <li> Run <code>./shash</code> </li>
    <li> Use the shell :) </li>
</ol>

<h3> File Structure </h3>

<ol>
    <li> <code>main.c</code> - Converts input to arguments, calls individual command executers </li>
    <li> <code>headers.h</code> - Header file included in all other header files and main. Contains external headers and global constants </li>
    <li> <code>echo.c and echo.h</code> - Implements echo </li>
    <li> <code>directory.c and directory.h</code> - Manages home and current directory, implements cd and pwd </li>
    <li> <code>ls.c and ls.h</code> - Implements ls</li>
    <li> <code>printprompt.c and printprompt.h</code> - Implements prompting (username@hostname) </li>
    <li> <code>run.c and run.h </code> - Implements foreground/background handling and execution of external commands </li>
    <li> <code>pinfo.c and pinfo.h</code> - Implements pinfo </li>
    <li> <code>nightswatch.c and nightswatch.h</code> - Implements nightswatch</li>
</ol>

<h4> Extra Features </h4>

<ol>
    <li> Getopt has been used to ensure proper argument handling </li>
    <li> Generalized signal handler based on signalTransfer for ease-of-extension in Assignment 3</li>
    <li> Fancier stuff not in requirements like colours, usage messages, arrow-key handling etc. will be coded as part of Assignment 3.</li>
</ol>


<h3> Assumptions </h3>

Apart from the requirements in the Assignment pdf, clarifications on moodle, and conforming to shell behaviour, the following assumptions were made:

<h4> System Requirements </h4>

<ol>
    <li> The code has not been tested on Mac, WSL etc. While it should work on most POSIX environments, it has only been tested in Ubuntu 20. </li>
</ol>

<h4> Input Restrictions </h4>

<ol>
<li> A maximum of 100 arguments (including command) are allowed. Each argument (including output of echo) does not exceed 1000 characters. 
<li> It is assumed targeted attempts to find uncommon bugs in the code won't be made. The code is not hack-proof and invalid input may lead to unwanted behaviour (including segfault).
<li> Semicolons have not been handled as arguments to any command or echo. They signify a command-break, aka, start of a new command.
<li> Error handling is not extremely rigorous for now, so in some cases missing permissions, changing behavior of certain files while execution etc. will lead to uncatched errors. </li>
<li>Nightswatch terminates on entering q and pressing enter</li>
<li>Multiple consecutive pipes don't work, i.e. ls||sort or ls | | sort etc. as the characters between them are themselves parsed as a command. Piping won't be provided for background processes. </li>
<li> An error has to be thrown on incorrect no. of arguments for kjob, bg too. </li>
</ol>


<h4> Process Management </h4>

<ol> 
    <li> Some background processes (like ps) give output on the terminal, and the prompt is not shown after these in bash. The same behaviour has been followed. </li>
    <li> The implementation of pinfo requires a certain format of proc/pid/status and proc/pid/exe which may not be available in every distribution. </li>
    <li> It is assumed that when the shell is closed, background processes have to be terminated (not normally) too. It is upto the background process to print any relevant information. Vim catches HUP, hopefully other processes you try will too :) </li>
</ol>