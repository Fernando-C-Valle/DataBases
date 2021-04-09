# Description

This repository contains projects that use PostgreSQL and MySQL as database
servers. The applications for this projects are not necessarily each other related.

## Linux Commands Database

This project uses a database with PostgreSQL, and its purpose is to store
linux commands and a short description about them, in order to keep them saved
 and ordered.

The 'main' files are independent applications, so they can be used in any preferred 
environment. 

The programming languages for the main files are Python and C++

### Tools

Developed with:

Database: PostgreSQL 13.2

OS: Manjaro ARM

Languages: Python 3.9.2, C++ 17

Device: Raspberry PI 4 Model B

### Execution 

The libraries implemented for the Python and C++ files are psycopg2 and libpqxx respectively.

The _Python_ 'main.py' file can be ran typing in a terminal:

```
python3 main.py
```

For the _C++_ 'main.cpp' file, the compilation command is:

```
g++ -Wall -Wextra -o -lpqxx -lpq main main.cpp
```