The Overfull Language
=================

Data types
----------

1. Strings: (str)\
    ```str name = "colby";```
2. Integers: (int)\
    ```int count = 5;```
3. Decimals: (dec)\
    ```dec pi = 3;```
4. Binary: (bin)\
    ```bin stop = true;```\
    **NOTE** 'yes' and 'no' can be substituted for true and false, respectively.

Unique features
---------------

1. All data types can have different variations:\
    **NOTE:** These variations will eventually control various attributes of a type but at the moment only control size.\
    Ex. ```int:64 time = 10;```

Example code (main.ofl)
------------

```
str test1 = "1",
    test2 = "2";

repeat 2 
{
    print test1;
    print test2;
    print test3;
}

Output:
-------
1
2
nil
1
2
nil
```

Usage
-----
1. Interpret a .ofl file:\
    ```ofl run main.ofl```
2. Compile a .ofl file:\
    ```ofl compile main.ofl```
3. Open the ofl interpreter on the command line:\
    ```ofl```

Installation
------------