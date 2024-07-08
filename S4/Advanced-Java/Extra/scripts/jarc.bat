@ECHO OFF

set tmpdir_root=%~dp0.tmp

set source_root=%~dp0..\java-solutions\
set artifacts_root=%~dp0..\..\java-advanced-2024\artifacts\

set implementor_source=info\kgeorgiy\ja\eremin\implementor\Implementor.java
set implementor_artifact=info.kgeorgiy.java.advanced.implementor.jar
set implementor_class=info.kgeorgiy.ja.eremin.implementor.Implementor

mkdir "%tmpdir_root%"

javac "%source_root%%implementor_source%" -cp "%artifacts_root%%implementor_artifact%" -d %tmpdir_root%

cd "%tmpdir_root%" && jar -c -f "./../%implementor_class%.jar" -m "./../MANIFEST.MF" ./* && cd "%~dp0"

rmdir "%tmpdir_root%" /s /q