// convert

const fs = require('fs');
const tr = require('../build/translate.js');
console.log("\n\n\n----trans", tr);
console.log("converter:");
//
const fromFile = process.argv[2] || null;
if (!fromFile || !fs.existsSync(fromFile)) {
    throw new Error("script file not found:" + fromFile);
}
let fromLang = fromFile.split('.').pop().toLowerCase().trim();
if (fromLang.length <= 0 || !TRANSLATE_LANGS_ARR.includes(fromLang)) {
    throw new Error("script lang not valid:" + fromLang);
}
let toLang = (process.argv[3] || "").toLowerCase().trim();
if (toLang.length > 0 && !TRANSLATE_LANGS_ARR.includes(toLang)) {
    throw new Error("target lang not valid:" + toLang);
}
console.log("converting:", fromFile);
let code = fs.readFileSync(fromFile, 'utf8');
let langues = toLang.length > 0 ? [toLang] : [];
let codes = translate_ug_code(fromLang, langues, code);
for (let lang in codes) {
    if (fromLang != lang) {
        let toFile = fromFile + "." + lang;
        fs.writeFileSync(toFile, codes[lang], 'utf8');
        console.log("converted:", toFile);
    }
}
console.log("finished!");