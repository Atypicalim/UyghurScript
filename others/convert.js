// convert

const fs = require('fs');
const path = require('path');
require('../build/translate.js');
require('../build/colorize.js');
console.log("converter:");
//
const fromFile = process.argv[2] || null;
let transDir = process.argv[3] || null;
let colorDir = process.argv[4] || null;
//
console.log("converting:", fromFile);
const pathInfo = path.parse(fromFile);
if (!transDir) transDir = pathInfo.dir;
if (!colorDir) colorDir = transDir;
// console.log(`converting.transDir:[${transDir}]`);
// console.log(`converting.colorDir:[${colorDir}]`);
//
const fromName = pathInfo.name;
const fromLang = pathInfo.ext.substring(1);
// console.log(`converting.fromName:[${fromName}]`);
// console.log(`converting.fromLang:[${fromLang}]`);
//
if (fromLang.length <= 0 || !TRANSLATE_LANGS_ARR.includes(fromLang)) {
    throw new Error("script lang not valid:" + fromLang);
}
if (!fromFile || !fs.existsSync(fromFile)) {
    throw new Error("script file not found:" + fromFile);
}
//
let code = fs.readFileSync(fromFile, 'utf8');
let langues = [];
let codes = translate_ug_code(langues, code);
for (let lang in codes) {
    let transFile = path.join(transDir, fromName) + "." + lang;
    let colorFile = path.join(colorDir, fromName) + "." + lang + ".html";
    let _code = codes[lang];
    let _html = colorize_ug_code(_code);
    fs.writeFileSync(transFile, _code, 'utf8');
    fs.writeFileSync(colorFile, _html, 'utf8');
    console.log("converted:", lang);
}
//
console.log("converted!");
