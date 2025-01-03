
let COLORIZE_PROJECT_REPO = '{}'; // [M[ PROJECT_REPO ]M]

let COLORIZE_COLOR_MAP = {
    // [M[ COLORS_MAP ]M]
};

let __code_bg_style = `
background: #272823;
color: #eff0ea;
font-family: Roboto,Arial,sans-serif,Alkatip;
`;

let __code_fg_colors = {
    red: "#f92f78",
    yellow: "#e7dc79",
    green: "#a1db27",
    blue: "#64d2e7",
    grey: "#8d7d4f",
    purple: "#a686ff",
    orange: "#fd8e2a",
};

Array.prototype.insert = function ( index, ...items ) {
    this.splice( index, 0, ...items );
};

let BRACKETS = ["()", "[]", "{}"];

function colorize_ug_code(code, isColorized) {
    let text = code.substring(0).trim();
    //
    let keys = Object.keys(COLORIZE_COLOR_MAP);
    keys.sort((a, b) => b.length - a.length);
    //
    function block2colorized(block) {
        for (let i = 0; i < keys.length; i++) {
            const key = keys[i];
            const color = COLORIZE_COLOR_MAP[key];
            if (key == block) {
                return `<${color}>${key}</${color}>`;
            }
        }
        if (!isNaN(block) && typeof Number(block) === 'number') {
            return `<purple>${block}</purple>`;
        }
        if (block.startsWith(`\\"`) && block.endsWith(`\\"`)) {
            var _block = block.substring(2, block.length - 2);
            return `<yellow>"${_block}"</yellow>`;
        }
        if ("+-*/%^".includes(block)) {
            return `<red>${block}</red>`;
        }

        let yBgn = "<yellow>";
        let yEnd = "</yellow>";
        let prefix = `<orange>@$1</orange>${yBgn}`;

        block = block.replaceAll(/@(\S+)\.([^\d\s][^\s]*)/gi, `${prefix}${yEnd}<yellow>.$2</yellow${yBgn}${yEnd}`);
        block = block.replaceAll(/@(\S+)\.(\d\d*)/gi, `${prefix}${yEnd}<purple>.$2</purple>${yBgn}${yEnd}`);
        block = block.replaceAll(/@(\S+)\{(\S+)\}/gi, `${prefix}{${yEnd}<white>$2</white>${yBgn}}${yEnd}`);
        return block;
    }
    //
    let chars = Array.from(text.replace(/\r\n/g, "\n"));
    let lines = [];
    let isString = false;
    let line = "";
    for (let i = 0; i < chars.length; i++) {
        const char = chars[i];
        if (char == `"`) {
            isString = isString == false ? true : false;
        }
        if (char != '\n' || isString) {
            line = line + char;
        } else {
            lines.push(line);
            line = "";
        }
    }
    if (line.length > 0) {
        lines.push(line);
        line = "";
    }
    //
    for (let i = 0; i < lines.length; i++) {
        let line = JSON.stringify(lines[i]).replaceAll(/^"/gi, "").replaceAll(/"$/gi, "");
        if (line.match("^\s*#")) {
            lines[i] = line.replaceAll(/#([^\n]+)/gi, `<grey>#$1</grey>`);
            continue;
        }
        let blocks = line.split(" ");
        // 
        for (let j = blocks.length-1; j >= 0; j--) {
            const block = blocks[j];
            for (let k = 0; k < BRACKETS.length; k++) {
                const brackets = BRACKETS[k];
                const opening = brackets[0];
                const closing = brackets[1];
                if (block.startsWith(opening)) {
                    blocks[j] = block.substring(1, block.length);
                    blocks.insert(j, "《" + opening + "》");
                }
                if (block.endsWith(closing)) {
                    blocks[j] = "《" + closing + "》";    
                    blocks.insert(j, block.substring(0, block.length-1));
                }
            }
        }
        // 
        for (let j = 0; j < blocks.length; j++) {
            const block = blocks[j];
            let translated = block2colorized(block);
            blocks[j] = translated;
        }
        line = blocks.join(" ");
        // 
        for (let k = 0; k < BRACKETS.length; k++) {
            const brackets = BRACKETS[k];
            const opening = brackets[0];
            const closing = brackets[1];
            line = line.replaceAll("《" + opening + "》 ", opening);
            line = line.replaceAll(" 《" + closing + "》", closing);
        }
        //
        lines[i] = line;
    }
    let _text = lines.join("\n");
    //
    _text = _text.replaceAll(/<([a-z]+)>/gi, function(match, p1, offset, string, groups) {
        let _styl = isColorized ? `style='color:` + __code_fg_colors[p1] + `;'` : "";
        return `<${p1} ${_styl} >`;
    });
    // 
    let _styl = isColorized ? `style="${__code_bg_style}"` : "";
    _text = `<pre ${_styl} >${_text}</pre>`;
    //
    return _text;
}


if (typeof global !== 'undefined') {
    global.colorize_ug_code = colorize_ug_code;
}
