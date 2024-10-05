
from scripts.base import *

############################################################################### translate

letterArray = []
for name, infos in mapName2LangLetters.items():
    lines = ""
    for lang, value in infos.items():
        lines = lines + "{}:\"{}\",".format(lang, value)
    letterArray.append(name + ":{"+ lines + "},")
letterText = "\n".join(letterArray)
#
langArray = []
for lang in langsArrConfigs:
    langArray.append("'" + lang + "'")
langText = ", ".join(langArray)

# translate
def _onMacro(code, command, argument = None):
    if command == "PROJECT_REPO":
        return code.format(PROJECT_REPO)
    elif command == "LETTERS_MAP":
        return letterText
    elif command == "LANGS_ARR":
        return langText
bldr = builder.code()
bldr.setName("TRANSLATE")
bldr.setInput("./others/translate.tpl.js")
bldr.setComment("//")
bldr.setOutput(DIR_BUILD + "translate.js")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### colorize

colorMap = grammars["GRAMMAR_COLORS"]

def getColor(name):
    for typ in grammars:
        names = grammars[typ]
        if name in names:
            return colorMap[typ]
    return "black"
#
colorArray = []
for name, infos in mapName2LangLetters.items():
    lines = ""
    for lang, value in infos.items():
        color = getColor(name)
        line = "\"{}\":\"{}\",".format(value, color)
        colorArray.append(line)
colorText = "\n".join(colorArray)
#
langArray = []
for lang in langsArrConfigs:
    langArray.append("'" + lang + "'")
langText = ", ".join(langArray)

# colorize
def _onMacro(code, command, argument = None):
    if command == "PROJECT_REPO":
        return code.format(PROJECT_REPO)
    elif command == "COLORS_MAP":
        return colorText
    elif command == "LANGS_ARR":
        return langText
bldr = builder.code()
bldr.setName("COLORIZE")
bldr.setInput("./others/colorize.tpl.js")
bldr.setComment("//", False)
bldr.setOutput(DIR_BUILD + "colorize.js")
bldr.onMacro(_onMacro)
bldr.onLine(lambda line: line)
bldr.start()

############################################################################### converter

bldr = builder.html()
bldr.setName("CONVERTER")
bldr.setInput("./others/converter.html")
bldr.containScript()
bldr.containStyle()
bldr.containImage()
bldr.setOutput(DIR_BUILD + "converter.html")
bldr.start()
