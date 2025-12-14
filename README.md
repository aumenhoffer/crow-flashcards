# crow-flashcards
Flashcard manager application written using the Crow HTTP framework

## Requirements
[Crow](https://github.com/CrowCpp/Crow)

## Building
Get gcc and make
Run `make` in the project root directory
The executable is a.out

## Usage
Set the environment variable `FCARDS_PATH` to set the path where the flashcards will be stored. Default is under ~/.local/share/fcards
Run the built executable
The app listens on port 8080
