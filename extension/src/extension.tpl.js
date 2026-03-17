const vscode = require('vscode');

// var someModule = require("./some.language"); // [M[ IMPORT_LANG ]M]

const repositoryUrl = "{}";  // [M[ PROJECT_REPO ]M]
const projectName = "{}"; // [M[ PROJECT_NAME ]M]
const languageVersion = "{}" // [M[ LANG_VERSION ]M]
const extensionVersion = "{}" // [M[ EXT_VERSION ]M]
const jumpUrl = repositoryUrl;
const helloMessage = `Hello from ${projectName}(${languageVersion}) extension(${extensionVersion})`;

function activate(context) {

	console.log(`extension "${projectName}" is activating ...`);

	try {
	
		let cmdHello = vscode.commands.registerCommand('extension.sayHello', () => {
			vscode.window.showInformationMessage(helloMessage);
		});
		let cmdJump = vscode.commands.registerCommand("extension.tryJump", () => {
			vscode.env.openExternal(vscode.Uri.parse(jumpUrl))
		});
		let cmdRun = vscode.commands.registerCommand('extension.tryRun', async () => {
			const myTerminal = vscode.window.createTerminal(`${projectName}`);
			myTerminal.show();
			myTerminal.sendText('git -v');
		})

		context.subscriptions.push(
			cmdHello, cmdJump, cmdRun
		);

		// someModule.run_activate(context); // [M[ RUN_ACTIVATE ]M]

	} catch (error) {
		console.log(`extension "${projectName}" err: ${error}`);
	}

	console.log(`extension "${projectName}" is activated!`);

}

function deactivate() { }

module.exports = {
	activate,
	deactivate
}
