# Key-value vault

Kye-value vault is for storing, retrieving, and managing associative arrays. Such a store is implemented in many programming languages as a container class dictionary.

# Building

- To get anything built get the `src` directory
- To build the command line interface executable file, use command `make cli`. This command required `3.22 version of CMake`. Once, the building finished, the `KeyValueVault` executable file eppears in `buid` directory.
- Bo build tests, use command `make tests`. This also command required `3.22 version of CMake`. Once, the building finished, the `KeyValueVault_tests` executable file eppears in `buid_tests` directory.


# Other Makefile targets

- `make check` - run linter and static code analyzer.
- `make clean` - removes all files probuced on any other target

# Command line interface usage

- To run CLI, it is required to choose vault type. There are two vault types: `binary_tree` and `hash_table`. Also alowed to use abbreviation `BT` and `HT`.
```
./KeyValueVault BT
```
- Then programm await user to input commands. To stop awaiting and exit type command `EXIT`.

## Commands
### Set new key value pair.
```
SET key1 Vasylisa Kadyk 1998 Uganda 359
```
To set lifetime use `EX <seconds>`. Once lifetime is expired the key-value pair will be unavailable.
```
SET key1 Vasylisa Kadyk 1998 Uganda 359 EX 120
```
### Get key value pair
```
GET key1
```
### Check key value pair for presence
```
EXISTS key1
```
### Delete existed key value pair
```
DELETE key1
```
### Update existed key value pair
Update whole data
```
UPDATE key1 Gleb Drozhji 1990 Novosibirsk 1000
```
Or partially update
```
UPDATE key1 - - - Novosibirsk 1000
```
### Get all keys list
```
KEYS
```
### Rename key value pair
```
RENAME key1 key2
```
### Get key value pair lifetime
```
TTL key1
```
### Find key by value
```
FIND - - - Novosibirsk 1000
```
### Show whole data stored
```
SHOWALL
```
### Upload data from file
```
UPLOAD tests/datasets/data_correct_200.txt
```
or for example of bad data
```
UPLOAD tests/datasets/data_incorrect_fault_on_13.txt
```
### Export data to file
```
EXPORT test_file.txt
```
