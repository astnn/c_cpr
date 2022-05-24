/* main.c */

/* By A. S. Nielsen 2022
This code is made available under a GNU GPL-3.0 (or later) license. The full
license is available in the file "COPYING" at the root of the repository. */

/*** includes ***/
#include <stdio.h>
#include <stdlib.h> // Suplies EXIT_FAILURE, EXIT_SUCCESS, exit()
#include <getopt.h> // For parsing command line options. getop() ect.
#include <libgen.h> // Supplies basename()
#include <errno.h> // Defines the external errno variable and possible values

#include "main.h"
#include "cpr.h"

/* defines */
#define OPTSTR "vi:o:h"
#define USAGE_FMT  "%s [-v] [-i inputfile] [-o outputfile] [-h]\n"
#define ERR_FOPEN_INPUT  "fopen(input, r)"
#define ERR_FOPEN_OUTPUT "fopen(output, w)"
#define ERR_GENERATE_CPR "Error encountered while running generateCpr()"
#define DEFAULT_PROGNAME "cpr"

/* external declarations */
extern int errno;
extern char *optarg;
extern int opterr, optind;

/* function prototypes */
void usage(char *progname, int opt);


/* main is responsible for the command line interface and hands over controll to
generateCprs() if the CLI input is succesfully read or return errors otherwise.
*/
int main(int argc, char *argv[]) {
  int opt;
  options_t options = {0, stdin, stdout};
  
  opterr = 0;
  
  // Handle command-line arguments
  while( (opt = getopt(argc, argv, OPTSTR)) != EOF) {
    printf("Opt: %c\n", opt);
    switch(opt) {
      case 'i':
        if(!(options.input = fopen(optarg, "r")) ) {
          perror(ERR_FOPEN_INPUT);
          exit(EXIT_FAILURE);
          /* NOTREACHED */
        }
        break;
      case 'o':
        if(!(options.output = fopen(optarg, "w")) ){
          perror(ERR_FOPEN_OUTPUT);
          exit(EXIT_FAILURE);
          /* NOTREACHED */
        }
        break;
      case 'v':
        options.verbose += 1;
        break;
      case 'h':
        /* Fallthrough */
      default:
        usage(basename(argv[0]), opt);
        /* NOTREACHED */
        break;
    }
  }
  
  // Run main code: generateCpr
  if(generateCprs(&options) != EXIT_SUCCESS) {
    perror(ERR_GENERATE_CPR);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
  } else {
    exit(EXIT_SUCCESS);
  }
}

/* usage: prints usage instructions to stderr */
void usage(char *progname, int opt) {
 fprintf(stderr, USAGE_FMT, progname?progname:DEFAULT_PROGNAME);
 exit(EXIT_FAILURE);
 /* NOTREACHED */
}
