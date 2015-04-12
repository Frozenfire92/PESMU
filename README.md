# PESMU
Notepad++ plugins for a first year CS Java build environment

# About
Programming Environment for Saint Mary's University(PESMU) is a project myself and 3 teammates worked on for our undergrad Software Engineering course. Our only requirement was to create a solution for a lightweight plugin to [Notepad++](http://notepad-plus-plus.org/) that would aid in teaching first year Java courses that would allow compilation and execution from within Notepad++.

We being the open source afficiandos we are, decided to extend existing open source projects to make our feature set a bit more complete. Each feature exists as its own plugin on its own branch in this repo.

### Compilation and Execution
We sent messages to the existing plugin [NPP Exec](http://sourceforge.net/projects/npp-plugins/files/NppExec/) to handle compilation and execution, as well as displaying a console window from within the editor.

### API Searching
We implemented a custom solution to search the Java API with the currently selected text, or to open the Java API directly

### Javadoc Generation
We modified [Doxyit](https://github.com/dail8859/DoxyIt) to only support Java and Javadoc, insert the File comment at the top, and recognize classes. It also interfaces with the Compilation and Execution plugin to generate the Javadoc HTML.

### Task List
We attempted to modify the existing plugin [NPP Task List](https://code.google.com/p/npp-task-list/) to support an extended interface that allowed removal of tasks from within the task list. This feature is mostly broken and incomplete

### Syntax Highlighting
We modified the default Notepad++ language file to support commonly used Java classes to aid students in understanding what is part of the language. For example `String`

##### Note
We used GitHub releases to download dlls before our presentation as school computers and other means had issues with it. Please ignore them.
