### c_cpr - a CPR number generator

This project implements the C program ```cpr``` that, given birth date and sex,
returns all **modulus 11 compliant** CPR numbers.

#### Requirements

- C compiler
- GNU make

#### Build

0. Clone

```bash
   $ git clone https://github.com/astnn/c_cpr.git
```

1. Compile
```bash
   $ make
```

2. Run
```bash
   $ ./cpr < input_file
```

#### Input and output
The program takes its input input either from stdin or from the file given after
the optional ´-i´ flag. Output is either returned to stdout or the file specified
by the optional ´-o flag.

The data format of the input birth date and sex must be given on the format
´DDMMYYYYS´, where ´DD´ is day, ´MM´ is month and ´YYYY´ is year. ´S´ represents
the sex and must be either 'M' for male or 'F' for female.
Multiple inputs can be given in the same file, so long as it each input is given
on its own line.

#### Usage example
```bash
   $ printf "01021999M\n03041998F\n" > test.txt
   $ ./cpr < test.txt
```

#### The CPR number
The Central Personregister (CPR) is a national Danish civil registry storing
information about all Danish citizens, and most other residents.

The CPR number is the 10 cipher unique identifier given to each entry in the
registry. It is broadly used to exchange personal information between public and
private IT systems and databases throughout Denmark.

For a more detailed description of the CPR number see its [Wikipedia entry][1]
or visit their [website][2] (in Danish).
The technical description, upon which this program has been build, can be found
[here][3] (also Danish).


[1]: https://en.wikipedia.org/wiki/Personal_identification_number_(Denmark)
[2]: https://cpr.dk/
[3]: https://cpr.dk/cpr-systemet/opbygning-af-cpr-nummeret/

#### Further work
0. Stricter input validation. Implement lookup-table to get highest day of month
(bonus points if it takes leap-years into consideration!).
1. Verbose program execution. Currently the verbose flag, -v, does not alter
program behavior.
2. Option for generation of non-modulus 11 compliant CPR numbers.
3. Command line interface option for checking if a given CPR number is a valid.

#### License

By A. S. Nielsen 2022.

All code in this repository is made available under a GNU GPL-3.0 (or later)
license. Anyone is free to use, modify, and redistribute the code under the 
terms of the license. The full license is available in the file "COPYING" at the
root of the repository.
