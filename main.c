// implementation

#define CARGS_IMPLEMENTATION
#include "libs/cargs/cargs.h"
#include "incbin.h"
#include "uyghur/uyghur.c"

#include <locale.h>
#include <wchar.h>

#define EXE_DELIMITER "#---UYGHUR---#"


void try_merge_win_program(char *executable, char *script) {
    //
    log_debug("packing executable...");
    char *target = "test.exe";
    char *cmd = tools_format("copy %s %s", executable, target);
    system(cmd);
    //
    log_debug("packing delimiter...");
    cmd = tools_format("echo: >> %s", target);
    system(cmd);
    system(cmd);
    cmd = tools_format("echo %s >> %s", EXE_DELIMITER, target);
    system(cmd);
    cmd = tools_format("echo: >> %s", target);
    system(cmd);
    //
    log_debug("packing script...");
    cmd = tools_format("type %s >> %s", script, target);
    system(cmd);
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
    int ignore = 1;
    int indent = strlen(EXE_DELIMITER);
    int number = 0;
    char* needle = strstr(content, EXE_DELIMITER);
    char* _needle = NULL;
    while (needle) {
        number = number + 1;
        _needle = needle;
        needle = strstr(needle + indent, EXE_DELIMITER);
    }
    return number > ignore ? _needle : NULL;
}

char *try_read_merged_script(CString name) {
    #ifdef IS_WINDOWS
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

void run_package_cmd(CString name, args_t args, CString script) {
    #ifdef IS_WINDOWS
        log_debug("packaging...");
        try_merge_win_program(name, script);
        log_debug("packaged!");
    #elif
        log_error("packaring not supported for: %s", PLATFORM_NAME);
    #endif
}

void run_execute_cmd(CString name, args_t args, CString script) {
    Uyghur *uyghur = Uyghur_instance();
    Uyghur_exePath(uyghur, script);
    Uyghur_free(uyghur);
}

// 

int main(int argc, char const *argv[])
{
    INCTXT(Script, UG_SCRIPT_PATH);
    // log
    log_set_file("./test.log");
    log_set_level(0);
    log_set_color(true);
    log_set_quiet(false);
    // locale
    popen("chcp 65001", "r");
    setlocale(LC_ALL, "en_US.utf8");
    // args
	args_t args = new_args(argc, argv);
	char *name = args_shift(&args);
	bool help    = false;
    bool version = false;
    char *package = NULL;
    char *execute = NULL;
	flag_bool("h", "help",    "show this help",        &help);
	flag_bool("v", "version", "print program version", &version);
	flag_cstr("p", "package", "package given script",  &package);
	flag_cstr("e", "execute", "execute given script",  &execute);
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
	} else if (package) {
        run_package_cmd(name, args, package);
		return EXIT_SUCCESS;
	} else if (execute) {
        run_execute_cmd(name, args, execute);
		return EXIT_SUCCESS;
    }
    // script
    char *script = try_read_merged_script(name);
    if (script == NULL) {
        script = gScriptData;
    }
    // program
    Uyghur *uyghur = Uyghur_instance();
    if (argc == 1) Uyghur_runCode(uyghur, script, NULL);
    if (argc >= 2) Uyghur_exePath(uyghur, (char *)argv[1]);
    Uyghur_free(uyghur);
    //
    //
    // 
    return 0;
}
