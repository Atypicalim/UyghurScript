
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
        if (block.startsWith('[') && block.endsWith(']')) {
            var _block = block.substring(1, block.length - 1);
            return `[<yellow>${_block}</yellow>]`;
        }
        if ("+-*/%^".includes(block)) {
            return `<red>${block}</red>`;
        }
        block = block.replaceAll(/@(\S+)\[(\S+)\]/gi, `<orange>@$1</orange>[<yellow>$2</yellow>]`);
        block = block.replaceAll(/@(\S+)\{(\S+)\}/gi, `<orange>@$1</orange>[<white>$2</white>]`);
        block = block.replaceAll(/@(\S+)\((\S+)\)/gi, `<orange>@$1</orange>[<purple>$2</purple>]`);
        return block;
    }
    //
    let chars = Array.from(text.replace(/\r\n/g, "\n"));
    let lines = [];
    let isString = false;
    let line = "";
    for (let i = 0; i < chars.length; i++) {
        const char = chars[i];
        if (char == "[") isString = true;
        if (char == "]") isString = false;
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
        for (let j = 0; j < blocks.length; j++) {
            const block = blocks[j];
            let translated = block2colorized(block);
            blocks[j] = translated;
        }
        lines[i] = blocks.join(" ");
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
