#define GS_NO_HIJACK_MAIN
#include "../gunslinger/gs.h"
#include <windows.h>
static HMODULE app_dll;
void (*app_init)(void);
void (*app_update)(void);

void app_load(){
    if(app_dll){
        FreeLibrary(app_dll);
    }
    if(CopyFile("app.dll","temp.dll",false))
    {
        app_dll = LoadLibraryA("temp.dll");
        app_init = (void* ) GetProcAddress(app_dll,"app_init");
        app_update =(void *) GetProcAddress(app_dll,"app_update");
    }
    gs_printf("app loaded \n");
}
void init()
{
    app_init();
}

void update()
{
    if(gs_platform_key_pressed(GS_KEYCODE_F5)){
        app_load();
        app_init();
    }
    else app_update();
}

int main(int argc, char *argv[])
{
    app_load();
    gs_app_desc_t app = {0}; // Fill this with whatever your app needs
    app.window_title = "App hot-reload example";
    app.update = update;
    app.init = init;
    gs_engine_create(app);
    while (gs_engine_app()->is_running) {
	    gs_engine_frame();
    }
    return 0;
}
