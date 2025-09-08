# TodoApp

A super fast system level todo App that works on the terminal

# Features

- Supports atomic file I/O file operations (your tasks stay safe)
- Uses a custom file format (.tsk) with a custom header and layout
- A clean CLI design, works natively like other linux commands

# Installation

## Clone this repo
```bash
git clone https://github.com/laaouina-anas/TodoApp.git
```
## Change directory to TodoApp
```bash
cd TodoApp
```
## Using gcc compiler, compile the source code to the bin directory
```bash
gcc -Iinclude -Wall -Wextra src/*.c -o bin/task
```

## Set enviroment variable

### Windows
include the absolute path of the /bin directory into user PATH
To do that:
- Press Win + R, type sysdm.cpl, press Enter
- Click "Environment Variables"
- Under "System variables" or "User variables", find "Path"
- Click "Edit", then "New"
- Add your directory path (e.g., C:\your\directory\path)
- Click OK to save

### Linux/mac
open the terminal, *inside the TodoApp directory*:
```bash
echo export PATH=$PATH:$PWD/bin/ >> ~/.bashrc
source ~/.bashrc
```
you are done !

## Note
Your tasks will be stored in the home directory, it is your default
directory when you first open the terminal, when you first use the program, a new
tasks.tsk file will be created, containing all your tasks in a binary format.
Each time you perform an operation, the file is updated!

## Usage

task
task [-h --help | -c --clear | -l --list]
task [-a --add | -d --delete] [list of strings or ids separated by space]
task [-m --modify] [id] [-s --status | -t --text] [text | [d | n]]

## Options

-h --help  : display this help output
-a --add   : add a list of tasks
-d --delete: delete a list of tasks by id -l --list  : print your task list
-c --clear : clear your task list, this action is permanent
-m --modify: modify a specific task by it's id
-s --status: for modifying the status, see usage above, 'd' is for done, 'n' is for not done
-t --text  : for modifying the text/description, see usage above

## Examples
```bash
task -a "my first task" "my second task" foo faa # add a list of tasks
task -l # list all your tasks
task --delete 3 2 1 # delete task for id = 1, 2 and 3
task --modify 1 --status d # modify task with id = 1 status to done
# you can combine more than one!
task -a "hello" world && task --list

# clear all your tasks (permanent)
task --clear # or task -c
```

## License

This project is licensed under the MIT License.