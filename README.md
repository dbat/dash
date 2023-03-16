# dash


 Synopsys:
   Get position of binary data in files (and replace with other data)

 Usage:
 
    dash [ -switches ] [ search-pattern ] [ filemasks... ]

    [switches] are as follows:
     -1|-0  use 1-based pos (default is 0-based)
     -a|-f  get all pos (default is only to get the first match)
     -i|-c  ignore case (default is case-sensitive)
     -m|-b  use simple memory scan (default is using boyer-moore algorithm)
     -x|-d  use hexadecimal offset (default is using decimal)

     -p NN  start from position NN
     -l NN  length/size to be processed (do not scan further)

            NN can be decimal, octal (0-prefix) or hexa (0x-prefix)

     -t     show translated pattern (default, if no filemask has given)
     -g     show progress and alert every 1GB scan increment on huge files
     -e     show Help about ESCAPED CHARACTERS

     --     stop evaluating the next arguments as switches
     -?     this Help

     Search and REPLACE:
     -s PAT (lowcase-s) search for this pattern:PAT instead
     -R PAT (CAPITAL-R) replace with this pattern:PAT
     -!     CONFIRM replace, otherwise only show what would be done

            if the search-pattern is specified explicitly with this switch
            then the rest arguments will be considered as filemask instead

            this pair of switches are built for convenience, so you can
            specify the search pattern first before the replacement pattern

            both patterns should yield an identical length, otherwise
            - if replacement is shorter, it only write to its length
            - if replacement is longer, the excess will be ignored

      NOTE: succesful replace will NOT update the file's date time.

    [pattern] is string which can contain ESCAPED CAHARACTERS as below:

    [filemask] can be a filename or wildcards (? and *, inc. dir portion)


    ESCAPED CHARACTERS:

    1.Equivalent with printf:
     \0 null byte
     \a alert
     \b backspace
     \f formfeed
     \n newline/linefeed (0A)
     \r carriage-return (0D)
     \t TAB (horizontal)
     \v vertical TAB

    2.Extension:
     \x[N] (locase-x),  [N] is hex, max 2 digits represents byte
     \X[N] (CAPITAL-X), [N] is hex, max 8 digits represents dword
     \y[N] (locase-y),  [N] is hex, max 4 digits represents word
     \Y[N] (CAPITAL-Y), [N] is hex, arbitrary length represents dword
     \d[N] (locase-d),  [N] is dec, arbitrary length represents byte (max=255)
     \D[N] (CAPITAL-D), [N] is dec, arbitrary length represents dword (max=4294967295)
     (any invalid digits will terminate/conclude the interpretation)

    3.Control characters:
     \c[C] where [C] is letter @A..Z[\]^_ represents CTRL-CHAR 00..1F
       \c@ = 0 (NULL)
       \cA = 1 [CTRL-A]
       \cC = 3 [CTRL-C]
       \cG = 7 [CTRL-G] (beep)
       \cI = 9 [CTRL-I] (TAB)
       \cZ = 26 [CTRL-Z] (EOF)
       ...
       \c^ = 30
       \c_ = 31
     (any invalid/unknown chars will terminate the interpretation)

