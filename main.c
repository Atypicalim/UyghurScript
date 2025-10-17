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
    printf("use of %s:\nOptions:\n", name);
	cargs_print(stdout);
}

void run_version_cmd(CString name) {
	printf("%s %s\n", name, UG_VERSION_NAME);
}

void run_interact_cmd(CString name, char *lang) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_runRepl(uyghur, lang);
    Uyghur_free(uyghur);
}

void run_execute_cmd(CString name, CString path, cArgs *args) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_runProgram(uyghur, path, args);
    Uyghur_free(uyghur);
}

void run_compile_cmd(CString name, CString path, CString lang) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_runCompile(uyghur, path, lang);
    Uyghur_free(uyghur);
}
void run_package_cmd(CString name, CString path) {
    #if IS_WINDOWS
        log_debug("packaging...");
        try_merge_win_program(path, name);
        log_debug("packaged!");
    #elif
        log_error("packaring not supported for: %s", PLATFORM_NAME);
    #endif
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
	cArgs args = cargs_new(argc, argv);
	char *name = args.path;
    //
    char *path = CARGS_REQUIRE_STRING;
    char *lang = "";
    //
	bool *help     = cargs_command("h", "help",     "show this help");
	bool *version  = cargs_command("v", "version",  "print program version");
    //
	bool *interact = cargs_command("i", "interact", "run interact mode");
    cargs_string(NULL, "lang", &lang);
	bool *execute = cargs_command("e", "execute",  "execute given script");
    cargs_string(NULL, "path", &path);
	bool *compile = cargs_command("c", "compile",  "compile given script");
    cargs_string(NULL, "path", &path);
    cargs_string(NULL, "lang", &lang);
	bool *package = cargs_command("p", "package",  "package given script");
    cargs_string(NULL, "path", &path);
    // 
	int err = cargs_run(&args);
    if (err == CARGS_UNKNOWN_COMMAND) {
        log_error("args unknown command name: %s", args.extra);
        return EXIT_FAILURE;
    } else if (err == CARGS_MISSING_PARAM) {
        log_error("args missing param name: %s", args.extra);
        return EXIT_FAILURE;
    } else if (err == CARGS_UNKNOWN_PARAM) {
        log_error("args unknown param name: %s", args.extra);
        return EXIT_FAILURE;
    } else if (err == CARGS_MISSING_VALUE) {
        log_error("args missing value for: %s", args.extra);
        return EXIT_FAILURE;
    } else if (err == CARGS_INVALID_VALUE) {
        log_error("args invalid value for: %s", args.extra);
        return EXIT_FAILURE;
    } else if (err != CARGS_OK && err != CARGS_MISSING_COMMAND) {
        log_error("args exception error code: %d", err);
		return EXIT_FAILURE;
	}
    //
	if (*help) {
        run_help_cmd(name);
		return EXIT_SUCCESS;
	} else if (*version) {
        run_version_cmd(name);
		return EXIT_SUCCESS;
	} else if (*interact) {
        run_interact_cmd(name, lang);
		return EXIT_SUCCESS;
	} else if (*execute) {
        run_execute_cmd(name, path, &args);
		return EXIT_SUCCESS;
	} else if (*compile) {
        run_compile_cmd(name, path, lang);
		return EXIT_SUCCESS;
	} else if (*package) {
        run_package_cmd(name, path);
		return EXIT_SUCCESS;
    }
    // script
    char *script = try_read_merged_script(name);
    if (script == NULL) {
        script = gScriptData;
    }
    // program
    Uyghur *uyghur = Uyghur_instance();
    CString _path = (char *)argv[1];
    if (argc <= 1) Uyghur_runScript(uyghur, script);
    if (argc >= 2) Uyghur_runProgram(uyghur, _path, &args);
    Uyghur_free(uyghur);
    // 
    return 0;
}
