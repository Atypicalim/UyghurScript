// implementation

#include "../my-build-tools/build/c_libs/incbin/incbin.h"
#include "uyghur/include.h"
#include "uyghur/uyghur.c"

#include <locale.h>
#include <wchar.h>

#define EXE_DELIMITER "#---UYGHUR---#"


void try_merge_win_program(char *executable, char *script) {
    // name
    char **parts = strsplit(script, "\\");
    int _index = -1;
    while (true) {
        if (!parts[_index + 1]) break;
        _index = _index + 1;
    }
    char *name = _index >= 0 ? parts[_index] : "unknown";
    char *target = tools_format("%s.exe", name);
    // executer
    log_debug("packing executer...");
    system_execute("copy %s %s", executable, target);
    // delimiter
    log_debug("packing delimiter...");
    system_execute("echo: >> %s", target);
    system_execute("echo: >> %s", target);
    system_execute("echo %s >> %s", EXE_DELIMITER, target);
    system_execute("echo: >> %s", target);
    // script
    log_debug("packing script...");
    system_execute("type %s >> %s", script, target);
}

char *try_read_win_script(CString name) {
    char* content = NULL;
    int length = 0;
    file_fetch(name, &content, &length);
    if (!length) return NULL;
    for (size_t i = 0; i < length; i++)
    {
        if (content[i] == '\0' && i < length - 1) content[i] = ' ';
    }
    char *lText = strfindl(content, EXE_DELIMITER);
    char *rText = strfindr(content, EXE_DELIMITER);
    return strlen(lText) != strlen(rText) ? rText : NULL;
}

char *try_read_merged_script(CString name) {
    #if IS_WINDOWS
        return try_read_win_script(name);
    #endif
    return NULL;
}

// 

void run_help_cmd(CString name) {
	args_print_usage(stdout, name, "[OPTIONS]");
}

void run_version_cmd(CString name) {
	printf("%s %s\n", name, UG_VERSION_NAME);
}

void run_interact_cmd(CString name) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_runRepl(uyghur);
    Uyghur_free(uyghur);
}

void run_package_cmd(CString name, args_t args, CString path) {
    #if IS_WINDOWS
        log_debug("packaging...");
        try_merge_win_program(name, path);
        log_debug("packaged!");
    #elif
        log_error("packaring not supported for: %s", PLATFORM_NAME);
    #endif
}

void run_execute_cmd(CString name, args_t args, CString path) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_runProgram(uyghur, path, args);
    Uyghur_free(uyghur);
}

void run_compile_cmd(CString name, args_t args, CString path) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_runCompile(uyghur, path, "js");
    Uyghur_free(uyghur);
}

// 

int main(int argc, char *argv[])
{
    INCTXT(Script, UG_SCRIPT_PATH);
    // log
    log_set_file(UG_IS_RELEASE ? NULL : "./test.log");
    log_set_level(UG_IS_RELEASE ? PCT_LOG_WARN : 0);
    log_set_color(true);
    log_set_quiet(false);
    // locale
    system_execute("chcp 65001");
    setlocale(LC_ALL, "en_US.utf8");
    // args
	args_t args = new_args(argc, argv);
	char *name = args_shift(&args);
	bool help    = false;
    bool version = false;
    bool interact = NULL;
    char *package = NULL;
    char *execute = NULL;
    char *compile = NULL;
	flag_bool("h", "help",     "show this help",        &help);
	flag_bool("v", "version",  "print program version", &version);
	flag_bool("i", "interact", "interact given script", &interact);
	flag_cstr("p", "package",  "package given script",  &package);
	flag_cstr("e", "execute",  "execute given script",  &execute);
	flag_cstr("c", "compile",  "compile given script",  &compile);
    // 
	int err = args_parse_flags(&args, NULL, NULL);
	if (err != ARG_OK) {
        log_error("args error: %d", err);
		return EXIT_FAILURE;
	}
	if (help) {
        run_help_cmd(name);
		return EXIT_SUCCESS;
	} else if (version) {
        run_version_cmd(name);
		return EXIT_SUCCESS;
	} else if (interact) {
        run_interact_cmd(name);
		return EXIT_SUCCESS;
	} else if (package) {
        run_package_cmd(name, args, package);
		return EXIT_SUCCESS;
	} else if (execute) {
        run_execute_cmd(name, args, execute);
		return EXIT_SUCCESS;
	} else if (compile) {
        run_compile_cmd(name, args, compile);
		return EXIT_SUCCESS;
    }
    // script
    char *script = try_read_merged_script(name);
    if (script == NULL) {
        script = gScriptData;
    }
    // program
    Uyghur *uyghur = Uyghur_instance();
    if (argc <= 1) Uyghur_runScript(uyghur, script);
    if (argc >= 2) Uyghur_runProgram(uyghur, (char *)argv[1], args);
    Uyghur_free(uyghur);
    //
    //
    // 
    return 0;
}
