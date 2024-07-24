
/**
 * simple node tool 
 * @returns selector func
 */
"use strict";

let CONVERT_LETTERS_MAP = {
// [M[ LETTERS_MAP ]M]
};

let CONVERT_LANGS_ARR = [
    // [M[ LANGS_ARR ]M]
];



function convert2language(from, to, code) {
    let text = code.substring(0);
    //
    let convertMap = {};
    Object.keys(CONVERT_LETTERS_MAP).forEach((key) => {
        let info = CONVERT_LETTERS_MAP[key];
        let fromLang = info[from];
        let toLang = info[to];
        convertMap[fromLang] = toLang;
    });
    let keys = Object.keys(convertMap);
    keys.sort((a, b) => b.length - a.length);
    //
    function block2translated(block) {
        for (let i = 0; i < keys.length; i++) {
            const key = keys[i];
            const value = convertMap[key];
            if (key == block) {
                return value;
            }
            // text = text.replaceAll(key, value);
        }
        return block;
    }
    //
    let lines = text.split("\n");
    for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        let blocks = line.split(" ");
        for (let j = 0; j < blocks.length; j++) {
            const block = blocks[j];
            let translated = block2translated(block);
            blocks[j] = translated;
        }
        lines[i] = blocks.join(" ");
    }
    text = lines.join("\n");
    //
    return text;
}
function convert2languages(from, langs, code) {
    let texts = {};
    if (!langs) langs = [];
    if (langs.length <= 0) langs = CONVERT_LANGS_ARR;
    for (let index = 0; index < langs.length; index++) {
        const lang = langs[index];
        texts[lang] = (convert2language(from, lang, code));
    }
    return texts;
}

let runConvert = typeof window == 'undefined' && typeof document == 'undefined';
if (runConvert) {
    const fs = require('fs');
    console.log("converter:");
    //
    const fromFile = process.argv[2] || null;
    if (!fromFile || !fs.existsSync(fromFile)) {
        throw new Error("script file not found:" + fromFile);
    }
    let fromLang = fromFile.split('.').pop().toLowerCase().trim();
    if (fromLang.length <= 0 || !CONVERT_LANGS_ARR.includes(fromLang)) {
        throw new Error("script lang not valid:" + fromLang);
    }
    let toLang = (process.argv[3] || "").toLowerCase().trim();
    if (toLang.length > 0 && !CONVERT_LANGS_ARR.includes(toLang)) {
        throw new Error("target lang not valid:" + toLang);
    }
    console.log("converting:", fromFile);
    let code = fs.readFileSync(fromFile, 'utf8');
    let langues = toLang.length > 0 ? [toLang] : [];
    let codes = convert2languages(fromLang, langues, code);
    for (let lang in codes) {
        if (fromLang != lang) {
            let toFile = fromFile + "." + lang;
            fs.writeFileSync(toFile, codes[lang], 'utf8');
            console.log("converted:", toFile);
        }
    }
    console.log("finished!");
}
