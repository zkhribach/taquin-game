
# Taquin Game

This is a **Taquin (sliding puzzle) game** developed in **C** using SDL2 for a school project. The game challenges players to arrange numbered tiles in the correct order by sliding them into an empty space.

## 🛠 Compilation

To compile the project, use the following command:

```bash
g++ game_logic.c render.c main.c -IC:\mingw_dev_lib\include\SDL2 -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o game
```

Make sure you have:
- **MinGW** installed
- The necessary **SDL2** and **SDL2_ttf** libraries in `C:\mingw_dev_lib\`

## 🎮 How to Play
1. Launch the game by running \`game.exe\`.
2. Navigate the menu using the arrow keys and enter.
3. Select a difficulty level and start solving the puzzle.
4. Arrange the tiles in order by sliding them into the empty space.

## 📌 Features
- Multiple difficulty levels.
- Timer to track completion time.
- A simple and intuitive interface.
- High score saving system.

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

