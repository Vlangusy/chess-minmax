# chess-minmax
- a chess game with only the basic rules - no stateful rules like castling, en passant, the 50 moves rule, also there is only automatic queening when a pawn reaches the end of the board
- the CPU opponent implements a multithreaded (Win32 API) minmax (or "minimax") algorithm
<img width="791" height="788" alt="chess-minmax-1" src="https://github.com/user-attachments/assets/c488ce7e-0f70-4ac7-bc34-136262cf30f4" />

https://github.com/user-attachments/assets/309bd29a-6916-44e1-8d8d-fe5635c6f62a
## compilation
- download the Raylib library (version 5.5 was used for this project) and drop raylib.lib into a lib\ folder in the project's root directory
- MSVC: `cl gdi32.lib msvcrt.lib lib\raylib.lib winmm.lib user32.lib shell32.lib source\chess.c source\drawing.c source\chessboard.c source\input.c source\minmax.c -Iinclude /Foobject\ /link /libpath:lib /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd`
