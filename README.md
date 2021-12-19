# UyghurScript

## 0. description

> a toy script interpreter written in c, u can write & run your script written in uyghur language grammar with it ...

## 1. data types and variable declaration

```python

# string
miqdar s qimmiti [Salam Uyghur Script!] bolsun

# number
miqdar n qimmiti 1 bolsun

# bool (rast, yalghan)
miqdar b qimmiti rast bolsun

# empty
miqdar d qimmiti quruq bolsun

```

## 2. expressions

```python

# assign a value
b qimmiti yalghan bolsun

# string expressions (teng, ulanghan)
s qimmiti [hi! ] ulanghan [boy ...] bolsun

# number expressions (teng, qushulghan, elinghan, kupeytilgen, bulungen)
n qimmiti n kupeytilghen 10 bolsun

# logic expressions (teng, kichik, chong)
b qimmiti a kichik 5 bolsun

# logic expressions (teng, hemde, yaki)
b qimmiti b hemde yalghan bolsun

```

## 3. operations

```python

miqdar sQimmet qimmiti [yalghan] bolsun

# transforming type (san, xet, quruq, logika, ekische, fonkisiye)
sQimmet qimmiti logika bolsun

# logic not
sQimmet qimmiti ekische bolsun

# print a logic value
ikrangha sQimmet yezilsun

```

## 3. io

```python

# output
ikrangha [Ana Tilim Zer Tilim!] yezilsun

# input
ikrandin a oqulsun
a qimmiti [mezmun:] ulanghan a bolsun
ikrangha a yezilsun

```

## 4. if

```python

miqdar a qimmiti quruq bolsun
miqdar b qimmiti rast bolsun
miqdar c qimmiti rast bolsun
miqdar d qimmiti [quruq!!!] bolsun

# the left side can be any value
eger a qimmiti rast bolsa
    ikrangha [eger] yezilsun
    d qimmiti [eger] bolsun
egerde b qimmiti rast bolsa
    ikrangha [egerde1] yezilsun
    d qimmiti [egerde1] bolsun
egerde c qimmiti rast bolmisa
    ikrangha [egerde2] yezilsun
    d qimmiti [egerde2] bolsun
bolmisa
    miqdar d qimmiti [ichide] bolsun
    ikrangha d yezilsun
    d qimmiti [bolmisa] bolsun
    ikrangha d yezilsun
tamamlansun

ikrangha d yezilsun

```

## 5. while

```python

miqdar yezildighanQimmet qimmiti [] bolsun
miqdar sinaqQimmet qimmiti 1 bolsun
miqdar tekshurulmeQimmet qimmiti rast bolsun

# the left side can be any value
nawada tekshurulmeQimmet qimmiti rast bolsa
    miqdar sinaqQimmetS qimmiti sinaqQimmet bolsun
    sinaqQimmetS qimmiti xet bolsun
    yezildighanQimmet qimmiti [nawada: sanaq ] ulanghan sinaqQimmetS bolsun
    ikrangha yezildighanQimmet yezilsun
    sinaqQimmet qimmiti sinaqQimmet qushulghan 1 bolsun
    tekshurulmeQimmet qimmiti sinaqQimmet kichik 5 bolsun
tamamlansun

```

## 6. functions

```python

# a variable for function result
miqdar yighinda qimmiti 0 bolsun

# function without args and return
fonkisiye sanYezish mezmuni
    ikrangha yighinda yezilsun
tamamlansun

# a function receiving two arguments and returning a result
fonkisiye sanQushush miqdar x y mezmuni
    miqdar waqitliqMiqdar qimmiti 0 bolsun
    waqitliqMiqdar qimmiti x qushulghan y bolsun
    netije waqitliqMiqdar qayturulsun
tamamlansun

# call function with two arguments and receive the result
fonkisiye sanQushush bilen 10 20 ishlitilsun we netije yighinda bolsun

# a string varibale storing a function name
miqdar funcName qimmiti [sanYezish] bolsun

# transform the string to a func which is the string value targeting
funcName qimmiti fonkisiye bolsun

# call a function without any argument and result 
fonkisiye funcName ishlitilsun

```

## 7. bridge

> u can get the bridge objet and communicate between c and script, just check `main.c` for more information

```c
// register c values to script
Bridge_startBox(bridge, NULL);
// push a key
Bridge_pushKey(bridge, "keyName");
// push a value (string, number, bool)
Bridge_pushValue(bridge, NULL);
// register
Bridge_register(bridge);
```

```c
// call script function from c
Bridge_startFunc(bridge, "functionName");
// push a value (string, number, bool)
Bridge_pushValue(bridge, NULL);
// call
Bridge_call(bridge);
// get rsult type
char *t = Bridge_topType(bridge);
// get result value
void *v = Bridge_popValue(bridge);

```

## 8. usage

* jsut run the command `./release/uyghur.exe ./examples/hello.ug` in root directory and you will see the result

## 9. todo

* box
* import
* traceback
* libraries
* ...

## 10. others

> i am still working on it ...
