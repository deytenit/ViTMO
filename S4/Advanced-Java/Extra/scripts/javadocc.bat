set javadoc_root=%~dp0..\javadoc\
set sourcepath=%~dp0..\java-solutions
set classpath=%~dp0..\..\java-advanced-2024\artifacts\info.kgeorgiy.java.advanced.implementor.jar

set subpackages=info.kgeorgiy.ja.eremin.implementor

set depsource=%~dp0..\..\java-advanced-2024\modules\info.kgeorgiy.java.advanced.implementor\

javadoc -sourcepath %sourcepath% -d %javadoc_root% -subpackages %subpackages% -cp %classpath% -private %depsource%info\kgeorgiy\java\advanced\implementor\*Impler*.java