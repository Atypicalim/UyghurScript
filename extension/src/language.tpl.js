const vscode = require('vscode');


const projectUrl = "{}";  // [M[ PROJECT_REPO ]M]
const targetLang = "{}"; // [M[ TARGET_LANG ]M]

const moduleMethodsMap = {
	// [M[ MODULE_METHODS ]M]
};

function try_complete_boxes(document, position) {
	// [M[ COMPLETE_BOXES ]M]
}

function try_complete_methods(document, position) {
	const prefix = document.lineAt(position).text.slice(0, position.character);
	let target = prefix.trim().split(' ').pop().slice(0, -1);
	// console.log("target", target);
	if (target == "" || target[0] != "@") {
		return;
	}
	const _target = target.substring(1);
	if (moduleMethodsMap.hasOwnProperty(_target)) {
		return moduleMethodsMap[_target][2];
	}
	return [];
}

function pack_complete_list(completes) {
	const _completes = [];
	for (let index = 0; index < completes.length; index++) {
		const element = completes[index];
		let complete = new vscode.CompletionItem(element, vscode.CompletionItemKind.Method);
		_completes.push(complete);
	}
	return _completes;
}

function run_activate(context) {

	console.log(`registering "${targetLang}" ...`);

	// autocomplete

	const myAutocomplete2 = vscode.languages.registerCompletionItemProvider(
		{ scheme: 'file', language: targetLang },
		{
			provideCompletionItems(document, position) {
				const completes = try_complete_boxes(document, position);
				return pack_complete_list(completes);
			}
		},
		'@',
	);

	const myAutocomplete1 = vscode.languages.registerCompletionItemProvider(
		{ scheme: 'file', language: targetLang },
		{
			provideCompletionItems(document, position) {
				const completes = try_complete_methods(document, position);
				return pack_complete_list(completes);
			}
		},
		'.',
	);

    // hover

	const myHover = vscode.languages.registerHoverProvider(
		{ scheme: 'file', language: targetLang }, {
		provideHover(document, position, token) {
			// parse
			const range = document.getWordRangeAtPosition(position);
			const word = document.getText(range);
			// find
			let moduleName = null;
			let urlPath = "";
			let kindName = "word";
			var isFound = false;
			for (const key of Object.keys(moduleMethodsMap)) {
				let conf = moduleMethodsMap[key];
				let folder = conf[0];
				let module = conf[1];
				let functions = conf[2];
				if (key == word) {
					kindName = "module";
					isFound = true;
				} else if (functions.includes(word)) {
					kindName = "function";
					isFound = true;
				}
				if (isFound) {
					moduleName = module;
					urlPath = `/tree/master/documents/${folder}/${module}.md`;
					break;
				}
			}
			// content
			const myContent = new vscode.MarkdownString(``);
			myContent.isTrusted = true;
			// seart
			const searchComponent = encodeURIComponent(JSON.stringify({ query: word }));
			const searchUri = vscode.Uri.parse(`command:workbench.action.findInFiles?${searchComponent}`);
			// document
			const documentPath = `${projectUrl}/${urlPath}`;
			const documentUri = vscode.Uri.parse(documentPath);
			// result
			myContent.appendMarkdown(`[search](${searchUri})`);
			myContent.appendMarkdown(` | `);
			myContent.appendMarkdown(`[document](${documentUri})`);
			myContent.appendMarkdown(` `);
			myContent.appendMarkdown(`*for* \`${word}\` *${kindName}*`);
			return new vscode.Hover(myContent);

		}
	});

	context.subscriptions.push(myAutocomplete1, myAutocomplete2, myHover);

}

module.exports = {
	run_activate,
}
