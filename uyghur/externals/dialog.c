// dialog

#include "../uyghur.c"

void native_dialog_inform(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString messageContent = Bridge_receiveStringWithDefault(bridge, "");
    tinyfd_messageBox(dialogTitle, messageContent, "yes", "info", 0);
    Bridge_returnEmpty(bridge);
}

bool native_dialog_confirm(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString messageContent = Bridge_receiveStringWithDefault(bridge, "");
    bool isYes = tinyfd_messageBox(dialogTitle, messageContent, "yesno", "question", 0) == 1;
    Bridge_returnBoolean(bridge, isYes);
}

void native_dialog_input(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString defaultText = Bridge_receiveStringWithDefault(bridge, NULL);
    CString resultText = tinyfd_inputBox(dialogTitle, "", defaultText);
    Bridge_returnString(bridge, resultText);
}

CString native_dialog_select_save(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString defaultPath = Bridge_receiveStringWithDefault(bridge, "./");
    CString filterRule = Bridge_receiveStringWithDefault(bridge, "*.*");
	char const * filterArray[1] = {filterRule};
    CString resultPath = tinyfd_saveFileDialog(dialogTitle, defaultPath, 1, filterArray, NULL);
    Bridge_returnString(bridge, resultPath);
}

CString native_dialog_select_open(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString defaultPath = Bridge_receiveStringWithDefault(bridge, "./");
    CString filterRule = Bridge_receiveStringWithDefault(bridge, "*.*");
	char const * filterArray[1] = {filterRule};
    CString resultPath = tinyfd_openFileDialog(dialogTitle, defaultPath, 1, filterArray, NULL, false);
    Bridge_returnString(bridge, resultPath);
}

CString native_dialog_select_folder(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString defaultPath = Bridge_receiveStringWithDefault(bridge, "./");
    CString resultPath = tinyfd_selectFolderDialog(dialogTitle, defaultPath);
    Bridge_returnString(bridge, resultPath);
}

CString native_dialog_select_color(Bridge *bridge) {
    CString dialogTitle = Bridge_receiveString(bridge);
    CString defaultColor = Bridge_receiveStringWithDefault(bridge, NULL);
    unsigned char const _chars[3] = "";
    CString resultColor = tinyfd_colorChooser(dialogTitle, defaultColor, _chars, _chars);
    Bridge_returnString(bridge, resultColor);
}
