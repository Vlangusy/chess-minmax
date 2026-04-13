# chess-minmax

## compilation
- drop raylib.lib into lib\
- `cl gdi32.lib msvcrt.lib lib\raylib.lib winmm.lib user32.lib shell32.lib source\chess.c source\drawing.c source\chessboard.c source\input.c -Iinclude /Foobject\ /link /libpath:lib /NODEFAULTLIB:libcmt /NODEFAULTLIB:msvcrtd`
