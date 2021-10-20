# UyghurScript

## 0. description

> a toy script interpreter written in c, u can run your script in uyghur language grammar with it ...

## 1. data types and variable declaration

```python

# only chars are supported for variable name, and all the variables are global
mixtar mixtarName qimmiti 0 bolsun

# string
mixtar a qimmiti [Salam Uyghur Script!] bolsun

# number
mixtar b qimmiti 1 bolsun

# bool (rast, yalghan)
mixtar c qimmiti rast bolsun

# nil
mixtar d qimmiti quruq bolsun

# assign a value
c qimmiti yalghan bolsun

# transform data type (number & string)
qimmet b tipi san bolsun
qimmet b tipi xet bolsun

```

## 2. operations

```python

# concat strings
a qimmiti [hi! ] ulanghan a bolsun

# calculate numbers (qushulghan, elinghan, kupeytilgen, bulungen)
b qimmiti a kupeytilghen 10 bolsun

# logic operations (kichik, chong, teng)
c qimmiti a kichik 5 bolsun

# logic operations (hemde, yaki, ekische)
c qimmiti c hemde yalghan bolsun

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

mixtar a qimmiti quruq bolsun
mixtar b qimmiti quruq bolsun
mixtar c qimmiti quruq bolsun

# eger (variable) qimmiti (variable|string|number|bool|nil) bolsa
eger a qimmiti rast bolsa
    ikrangha [eger] yezilsun
egerde b qimmiti rast bolsa
    ikrangha [egerde1] yezilsun
egerde c qimmiti rast bolsa
    ikrangha [egerde2] yezilsun
bolmisa
    ikrangha [bolmisa] yezilsun
tamamlansun

```

## 5. while

```python

# u can write an if statement in it
mixtar d qimmiti 0 bolsun
# nawada (variable) qimmiti (variable|string|number|bool|nil) bolsa
nawada c qimmiti rast bolsa
    d qimmiti d qushulghan 1 bolsun
    ikrangha d yezilsun
    c qimmiti d kichik 5 bolsun
tamamlansun

```

## 6. functions

```python

# variable
mixtar yighinda qimmiti 0 bolsun

# function without args and return
fonkisiye sanYezish mezmuni
    ikrangha yighinda yezilsun
tamamlansun

# function with variables and return
fonkisiye sanQushush mixtar x y mezmuni
    mixtar waqitliqMixtar qimmiti 0 bolsun
    waqitliqMixtar qimmiti x qushulghan y bolsun
    netije waqitliqMixtar qayturulsun
tamamlansun

# call with variables and receive result
fonkisiye sanQushush bilen 10 20 ishlitilsun we netije yighinda bolsun

# call without variable, and not result 
fonkisiye sanYezish ishlitilsun

# parse from a string
mixtar isim qimmiti [Yezish] bolsun
qimmet isim tipi fonkisiye bolsun
fonkisiye isim ishlitilsun

```

## 7. usage

* just run the command `uyghur.exe test.ug` to execute your script

## 8. todo

* box
* import
* libraries
* c & uyghur bridge
* ...

## 9. end

> i am still working on it ...
