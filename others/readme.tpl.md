# UyghurScript

## 0. description

> a toy script interpreter written in c, u can write & run your script written in uyghur language grammar with it ...

> 

* read [documents](./documents/) for more informations


* run `./release/uyghur.exe` to get help content

* cick `./release/yuguresh.exe` to run raylib example

![example](./resources/examples/running.png)

## 1. multiple langauge

// [M[ MD_LANGUAGES ]M]

## 2. highlight & translating

* vscode [extension](https://marketplace.visualstudio.com/publishers/Atypicalim) for highlighting and autocompleting.

* language [converter](https://Atypicalim.github.io/others/converter.html) for translating different languages.

## 3. supported features

// [M[ MD_REATURES ]M]

--------------------------------

## 4. supported libraries

* [internals](./documents/internals)

* [externals](./documents/externals)

## 5. bridge interfaces

> u can get the bridge objet and communicate between c and script, just check `bridge.c` for more information

*  register a box to script (with some keys of different types)
```c++
Bridge_startBox(bridge);
Bridge_bindBoolean(bridge, "bol", false);
Bridge_bindNumber(bridge, "num", 123);
Bridge_bindString(bridge, "str", "abc...");
Bridge_register(bridge, "boxName"); // NULL
```

* call script function from c (giving one argument and receiving result)
```c++
Bridge_startFunc(bridge);
Bridge_pushValue(bridge, Value_newNumber(123));
Bridge_pushValue(bridge, Value_newString(""));
Bridge_call(bridge, "funcName");
char resultType = Bridge_topType(bridge);
void *resultValue = Bridge_receiveValue(bridge);
```

* register c function for script (receiving two argument and giving result)
```c++
void testFunc(Bridge *bridge)
{
    int a = Bridge_receiveNumber(bridge);
    char *b = Bridge_receiveString(bridge);
    Bridge_returnBoolean(bridge, false);
}
Bridge_startBox(bridge);
Bridge_bindNative(bridge, "testFunc", testFunc);
Bridge_register(bridge, NULL);
```

## 6. todo

* libraries
* ...

## 7. others

> i am still working on it ...
