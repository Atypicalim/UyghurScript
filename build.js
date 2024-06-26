
let builder = require("../my-build-tools");
let tools = builder.tools;

let SCRIPT_PATH = "./examples/yuguresh.ug";

let DST_DIR = "./build/";
let DST_NAME = "uyghur";
let DST_ALIAS = DST_DIR + DST_NAME;
let DST_SCRIPT = DST_ALIAS + ".ug";

tools.files.mk_folder(DST_DIR)
tools.files.delete(DST_SCRIPT)
if (SCRIPT_PATH != null && tools.files.is_file(SCRIPT_PATH)) {
    tools.files.copy(SCRIPT_PATH, DST_SCRIPT);
} else {
    let help = " # yardem \n ikrangha [merhaba, uyghur script qa xosh kepsiz ...] yezilsun\n";
    tools.files.write(DST_SCRIPT, help)
}

let task = builder.c({})
task.setDebug(false)
task.setInput('./main.c')
task.setLibs("incbin", "raylib", "raygui")
task.setOutput(DST_ALIAS)
task.setIcon('./resources/icon.ico')
task.start(false)
task.run()

// test ...
