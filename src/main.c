#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "sokol/sokol_fetch.h"
#include "sokol/sokol_log.h"
#define USE_DBG_UI
#include "cdbgui/cdbgui.h"
#include "cdbgui/cdbgui.c"

#include "shaders.glsl.h"

#define SAMPLE_COUNT 4

static struct {
  sg_pipeline pip;
  sg_bindings bind;
  sg_pass_action pa;
} state;

static void init(void) {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
      .logger.func = slog_func,
  });

  float vertices[] = {
      0.0f, 0.5f, 0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f,
      1.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  1.0f,
  };
  state.bind.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      .data = SG_RANGE(vertices),
  });

  state.pip = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
      .layout =
          {
              .attrs =
                  {
                      [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
                      [ATTR_vs_color0].format = SG_VERTEXFORMAT_FLOAT4,
                  },
          },
  });

  state.pa = (sg_pass_action){
      .colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                    .clear_value = {0.1f, 0.1f, 0.15f, 1.0f}},
  };
  __cdbgui_setup(SAMPLE_COUNT);
}

static void frame(void) {
  sg_begin_pass(&(sg_pass){.action = state.pa, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pip);
  sg_apply_bindings(&state.bind);
  sg_draw(0, 3, 1);
  
  __cdbgui_draw();
  sg_end_pass();

  sg_commit();
}

static void cleanup(void) {
  __cdbgui_shutdown();
  sg_shutdown();
}

static void event(const sapp_event* e) {
  if ((e->type == SAPP_EVENTTYPE_KEY_DOWN) &&
      (e->key_code == SAPP_KEYCODE_ESCAPE)) {
    sapp_request_quit();
  }
  __cdbgui_event(e);
}

sapp_desc sokol_main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  return (sapp_desc){
      .cleanup_cb = cleanup,
      .event_cb = event,
      .frame_cb = frame,
      .init_cb = init,
      .width = 1280,
      .height = 720,
      .sample_count = SAMPLE_COUNT,
      .window_title = "Test Sokol",
      .icon.sokol_default = true,
      .logger.func = slog_func,
  };
}