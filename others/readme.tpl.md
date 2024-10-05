# UyghurScript

## 0. description

> a toy script interpreter written in c, u can write & run your script written in uyghur language grammar with it ...

> 

* run command `./release/uyghur.exe` to get help content

* double cick `./release/yuguresh.exe` to run raylib example

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

> read [documents](./documents/) for libraries including internal & external

## 5. bridge interfaces

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

## 6. todo

* libraries
* ...

## 7. others

> i am still working on it ...
