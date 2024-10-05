
from scripts.base import *

############################################################################### readme

tplMdLangauge = '''
* {alpha}. [{tran}]({path})

![](./resources/languages/hello.{lang}.png)
'''

tplMdFeature = '''* [{name}]({path})

```powershell
{code}
```
'''

print("README:")

mdLangaugesArray = []
i = 0
for lang in langTrans:
    tran = langTrans[lang]
    i = i + 1
    alpha = chr(ord('a') + i - 1)
    path = tools.tools.append_path(DIR_LANGUAGE, lang).replace("\\", "/")
    text = tplMdLangauge.format(alpha=alpha, lang=lang, tran=tran, path=path)
    mdLangaugesArray.append(text)
mdLanguagesText = "\n".join(mdLangaugesArray)

mdfeaturesArray = []
for name in features:
    if name == "hello":
        continue
    path = tools.tools.append_path("./examples/features/", name) + ".en"
    code = tools.files.read(path, 'utf-8').strip()
    path = tools.tools.append_path(DIR_FEATURES, name).replace("\\", "/")
    text = tplMdFeature.format(name=name, code=code, path=path)
    mdfeaturesArray.append(text)
mdFeaturesText = "\n".join(mdfeaturesArray)

def _onMacro(code, command, argument = None):
    if command == "MD_LANGUAGES":
        return mdLanguagesText
    elif command == "MD_REATURES":
        return mdFeaturesText
bldr = builder.code()
bldr.setName("README")
bldr.setInput("./readme.tpl.md")
bldr.setComment("//", False)
bldr.setOutput("./readme.md")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()
