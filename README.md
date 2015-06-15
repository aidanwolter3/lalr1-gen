# lalr1-gen
A LALR(1) parse table generator used for writing languages for my [SLP compiler](https://github.com/aidanwolter3/slp-compiler).

The language definition requires a specific syntax and the token IDs specified in this file should match up with a corresponding lexem table. Examples of these files are ```language_definition``` and ```lex_table.csv```. The output is written to a file ```parse_table.csv```.

# usage
```bash
$ ./lalr1-gen [language_definition_file]
```
