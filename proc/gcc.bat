gcc -shared -o gunslinger.dll source\impl.c -O0 -lopengl32 -luser32 -lgdi32 -lshell32 -lwinmm 
gcc -shared -o app.dll source\app.c -O0 gunslinger.dll
gcc source\host.c -o host.exe -O0 gunslinger.dll