
swf crumb

・このソフトについて
拡張子がswdのデバッグ情報ファイルを利用して、ActionScriptのtraceにファイル名と行番号を付加した改変を行ったswfを出力するソフトです。

・使い方
コマンドラインに3つのファイル名を渡して実行して下さい。

swdファイル名 入力swfファイル名 出力swfファイル名

・制限、バグ
ActionScript3には対応していません。
ActionConstantPoolを使っていない為、出力swfのファイルサイズが増えやすいです。
DefineFunction2には対応していません。パブリッシュ設定のデバッグを許可にチェックを入れるとデバッグ情報ファイルのswdが作られますが、この際swf中のActionScriptの最適化は行われず、DefineFunctionのみが使われるようです。（Flash CS6で確認）

・変更履歴

2013年1月2日 最初のバージョン
2013年1月3日 入力のswfがzlib圧縮されていたら出力するswfもzlib圧縮するように処理を追加

