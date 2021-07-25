
#include "../gunslinger/gs.h"


gs_command_buffer_t                      cb      = {0};
gs_handle(gs_graphics_vertex_buffer_t)   vbo     = {0};
gs_handle(gs_graphics_pipeline_t)        pip     = {0};
gs_handle(gs_graphics_shader_t)          shader  = {0};

__declspec(dllexport) void app_init();
__declspec(dllexport) void app_update();
// Shaders
const char* v_src =
"#version 330 core\n"
"precision mediump float;\n"
"layout(location = 0) in vec2 a_pos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(a_pos, 0.0, 1.0);\n"
"}";

const char* f_src =
"#version 330 core\n"
"precision mediump float;\n"
"out vec4 frag_color;\n"
"void main()\n"
"{\n"
"   frag_color = vec4(1.0, 1.0, 0.0, 1.0);\n"
"}";

// Vertex data for triangle
float v_data[] = {
    0.0f, 0.5f,
    -0.5f, -0.5f, 
    0.5f, -0.5f
};


void app_init()
{
    // Construct new command buffer
    cb = gs_command_buffer_new(); 

    // Construct vertex buffer
    vbo = gs_graphics_vertex_buffer_create(
        &(gs_graphics_vertex_buffer_desc_t) {
            .data = v_data,
            .size = sizeof(v_data)
        }
    );

    // Create shader
    shader = gs_graphics_shader_create (
        &(gs_graphics_shader_desc_t) {
            .sources = (gs_graphics_shader_source_desc_t[]) {
                {.type = GS_GRAPHICS_SHADER_STAGE_VERTEX, .source = v_src},
                {.type = GS_GRAPHICS_SHADER_STAGE_FRAGMENT, .source = f_src}
            }, 
            .size = 2 * sizeof(gs_graphics_shader_source_desc_t),
            .name = "triangle"
        }
    );

    pip = gs_graphics_pipeline_create (
        &(gs_graphics_pipeline_desc_t) {
            .raster = {
                .shader = shader
            },
            .layout = {
                .attrs = (gs_graphics_vertex_attribute_desc_t[]){
                    {.format = GS_GRAPHICS_VERTEX_ATTRIBUTE_FLOAT2, .name = "a_pos"}    // Named attribute required for lower GL versions / ES2 / ES3
                },
                .size = sizeof(gs_graphics_vertex_attribute_desc_t)
            }
        }
    );
}

void app_update()
{
    if (gs_platform_key_pressed(GS_KEYCODE_ESC)) gs_engine_quit();

    // Render pass action for clearing the screen
    gs_graphics_clear_desc_t clear = (gs_graphics_clear_desc_t){
        .actions = &(gs_graphics_clear_action_t){.color = {0.1f, 0.1f, 0.1f, 1.f}}
    };

    // Binding descriptor for vertex buffer
    gs_graphics_bind_desc_t binds = {
        .vertex_buffers = {&(gs_graphics_bind_vertex_buffer_desc_t){.buffer = vbo}}
    };
    gs_vec2 fbs = gs_platform_framebuffer_sizev(gs_platform_main_window());
    gs_graphics_set_viewport(&cb, 0,0,fbs.x,fbs.y);
    gs_graphics_begin_render_pass(&cb, GS_GRAPHICS_RENDER_PASS_DEFAULT);           // Begin render pass (default render pass draws to back buffer)
        gs_graphics_clear(&cb, &clear);                                            // Clear screen
        gs_graphics_bind_pipeline(&cb, pip);                                       // Bind our triangle pipeline for rendering
        gs_graphics_apply_bindings(&cb, &binds);                                   // Bind all bindings (just vertex buffer)
        gs_graphics_draw(&cb, &(gs_graphics_draw_desc_t){.start = 0, .count = 3}); // Draw the triangle
    gs_graphics_end_render_pass(&cb);                                              // End the render pass

    // Submit command buffer (syncs to GPU, MUST be done on main thread where you have your GPU context created)
    gs_graphics_submit_command_buffer(&cb);
}






