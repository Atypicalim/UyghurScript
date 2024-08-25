# UyghurScript

## 0. description

> a toy script interpreter written in c, u can write & run your script written in uyghur language grammar with it ...

> 

* run command `./release/uyghur.exe` to get help content

* double cick `./release/yuguresh.exe` to run raylib example

![example](./resources/examples/running.png)

## 1. multiple langauge


* a. english

![](./resources/languages/hello.en.png)


* b. ئۇيغۇرچە

![](./resources/languages/hello.uy.png)


* c. uyghurche

![](./resources/languages/hello.ug.png)


* d. o'zbekcha

![](./resources/languages/hello.uz.png)


* e. Türkçe

![](./resources/languages/hello.tr.png)


* f. қазақша

![](./resources/languages/hello.kz.png)


## 2. highlight & translating

* vscode [extension](https://marketplace.visualstudio.com/publishers/kompasim) for highlighting and autocompleting.

* language [converter](https://kompasim.github.io/others/converter.html) for translating different languages.

## 3. supported features

* variable

```powershell
# declare a dynamic variable
miqdar m qimmiti quruq bolsun

# declare a static variable
miqdar n qimmiti san bolsun

# set value
m = [salam dunya!]

# free variable
m qimmiti quruq bolsun

# type error
n = [sinaq...]
```

* type

```powershell
miqdar m qimmiti [yezish] bolsun

# get func by string
m qimmiti xizmetchi bolsun
buyruq m yezilsun

# convert value to boolean
m qimmiti logika bolsun
buyruq m yezilsun
```

* calculation

```powershell
# arthimetic
x = 1 + 2 # 3
x = 1 + 2 * 3 # 9
x = 1 + (2 * 3) # 7
x = 64 ^ (1 / 3) # 4

# bitwise
x = 3 & 2 # 2
x = 3 | 2 # 3
x = 3 ~ 2 # 1

# boolean
x = rast & yalghan # yalghan
x = rast | yalghan # rast
x = rast ~ yalghan # rast

# logic
x = [2] ? 2 # false
x = 2 < 3 # true
x = [ab] > [ac] # false 

# string
x = [ab] + [cd] # abcd
x = [ab] * 2 # abab

# checking
x = 10 % 7 # 3
x = 10 % san # true
x = 10 % quruq # false
```

* command

```powershell
# read a value
buyruq a oqulsun

# print a value
buyruq a yezilsun
```

* if

```powershell
eger 0 bolsa
    buyruq [if] yezilsun
egerde rast bolsa
    buyruq [elif] yezilsun
bolmisa
    buyruq [else] yezilsun
tamamlansun
```

* while

```powershell
miqdar i qimmiti 0 bolsun
nawada i < 9 bolsa
    buyruq [while\n] yezilsun
    i = i + 1
tamamlansun
```

* spread

```powershell
yeyilghan [abc...] miqdar i v bulup
    buyruq i yezilsun
    buyruq [\t] yezilsun
    buyruq v yezilsun
    buyruq [\n] yezilsun
tamamlansun
```

* worker

```powershell
# a local variable
miqdar t qimmiti 0 bolsun

# a function with two arguments
xizmetchi f miqdar x y mezmuni
    t = x + y
    netije t qayturulsun
tamamlansun

# call functions and receive result
xizmetchi f bilen 10 20 ishlitilsun we r elinsun

# print the result value received
buyruq r yezilsun
```

* exception

```powershell
miqdar x qimmiti quruq bolsun

# try block
xataliq e bulup
    x = 3 / 0
tamamlansun

# catch block
eger e bolsa
    buyruq [exception:\n] yezilsun
    buyruq e yezilsun
    buyruq [\n] yezilsun
bolmisa
    buyruq [successful!\n] yezilsun
tamamlansun
```

* container

```powershell
#  declare a list
t qimmiti tertip bolsun
#  declare a dict
s qimmiti sanduq bolsun

# generate a list
t = {11, 22, [abc]}
# generate a dict
s = {a: [aa], b: [bb], c: 123}

# put a number key
@t{1} = 456
# put a string key
@s[k] = [abc]
# put a variable key
@s{v} = val

# alias: this: - bu, module: + bolek, global: * dunya

# check type
b = t % tertip
```

* objective

```powershell
# an assister with one argument
yardemchi X miqdar x mezmuni
    @-[x] = x
tamamlansun

# assister func
xizmetchi @X[test] mezmuni
    buyruq [X.test...\n] yezilsun
tamamlansun

# a creator with one arguments
yasighuchi Y miqdar y mezmuni
    yasighuchi X bilen [arg xxx ...\n] ishlitilsun 
    @-[y] = y
tamamlansun

# creator func
xizmetchi @Y[test] mezmuni
    xizmetchi @X[test] ishlitilsun
    buyruq [Y.test...\n] yezilsun
tamamlansun

# call creator using an assister and receive object
yasighuchi Y bilen [arg yyy ...\n] ishlitilsun we r elinsun

# print the object value received
buyruq @r[x] yezilsun # arg xxx ...
buyruq @r[y] yezilsun # arg yyy ...

# call creator func which calling assister func inside
xizmetchi @r[test] ishlitilsun
# X.test...
# Y.test...

# check obj is created by or assisted by
eger r % X bolsa
    buyruq [yes\n] yezilsun
tamamlansun
# yes
```

* import

```powershell
# other.xx
miqdar x qimmiti [other...] bolsun

# main.xx
xizmetchi ekirish bilen [../other.xx] ishlitilsun we m elinsun
buyruq @m[x] yezilsun

# run the program
`.uyghur.exe ./main.xx`

# get the result
other...
```


--------------------------------

## 4. bridge interfaces

> u can get the bridge objet and communicate between c and script, just check `bridge.c` for more information

*  register a box to script
```c++
Bridge_startBox(bridge);
Bridge_bindValue(bridge, "num", "text...");
Bridge_bindValue(bridge, "str", "text...");
Bridge_register(bridge, "boxName"); // NULL for global scope
```

* call script function from c, and get the result
```c++
Bridge_startFunc(bridge);
Bridge_pushValue(bridge, "argument");
Bridge_call(bridge, "functionName");
char resultType = Bridge_topType(bridge);
void *resultValue = Bridge_receiveValue(bridge);
```

* call c function from script, and return result
```c++
void testFunc(Bridge *bridge)
{
    int a = Bridge_receiveNumber(bridge);
    int b = Bridge_receiveNumber(bridge);
    int c = a + b;
    Bridge_returnNumber(bridge, c);
}
Bridge_startBox(bridge);
Bridge_bindNative(bridge, "sinaqFonkisiye", testFunc);
Bridge_register(bridge, NULL);
```

## 5. todo

* objective
* libraries
* ...

## 6. others

> i am still working on it ...
