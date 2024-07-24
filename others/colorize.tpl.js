 
let COLORIZE_COLOR_MAP = {
    // [M[ COLORS_MAP ]M]
};

function colorize_ug_code(code) {
    let text = code.substring(0);
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
            _block = block.substring(1, block.length - 1)
            _block = _block.replaceAll("\\n", "N");
            return `[<yellow>${_block}</yellow>]`;
        }
        if ("+-*/%^".includes(block)) {
            return `<red>${block}</red>`;
        }
        block = block.replaceAll(/@(\S+)\[(\S+)\]/gi, `@<green>$1</green>[<yellow>$2</yellow>]`);
        block = block.replaceAll(/@(\S+)\{(\S+)\}/gi, `@<green>$1</green>[<cyan>$2</cyan>]`);
        block = block.replaceAll(/@(\S+)\((\S+)\)/gi, `@<green>$1</green>[<purple>$2</purple>]`);
        return block;
    }
    //
    let lines = text.split("\n");
    for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
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
    return `<pre>${_text}</pre>`;
}

if (typeof global !== 'undefined') {
    global.colorize_ug_code = colorize_ug_code;
}
