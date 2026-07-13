rem cmd.pkg からtoluaを使ってグルーコードを生成する

rem batファイルのあるフォルダを取得し、そこをカレントフォルダとする（VisualStudioなど別フォルダから起動される用に）
set batcurrent=%~dp0
cd %batcurrent%

rem グルーコード生成
tolua.exe -o luaglue_cmd.cpp cmd.pkg
