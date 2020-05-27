// Copyright 2020 Valentin Vanelslande
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#ifdef _WIN32
#define VVCTRE_PLUGIN_EXPORT __declspec(dllexport)
#else
#define VVCTRE_PLUGIN_EXPORT
#endif

#include "common_types.h"

static const char* required_function_names[] = {
    "vvctre_gui_begin_tab",
    "vvctre_gui_end_tab",
    "vvctre_gui_checkbox",
    "vvctre_get_cfg_module",
    "vvctre_settings_set_console_model",
    "vvctre_settings_get_console_model",
    "vvctre_settings_write_config_savegame",
};

VVCTRE_PLUGIN_EXPORT int GetRequiredFunctionCount() {
    return 7;
}

VVCTRE_PLUGIN_EXPORT const char** GetRequiredFunctionNames() {
    return required_function_names;
}

typedef bool (*vvctre_gui_begin_tab_t)(const char* label);
typedef void (*vvctre_gui_end_tab_t)();
typedef bool (*vvctre_gui_checkbox_t)(const char* label, bool* checked);
typedef void* (*vvctre_get_cfg_module_t)(void* core, void* plugin_manager);
typedef void (*vvctre_settings_set_console_model_t)(void* cfg, u8 value);
typedef u8 (*vvctre_settings_get_console_model_t)(void* cfg);
typedef void (*vvctre_settings_write_config_savegame_t)(void* cfg);

static vvctre_gui_begin_tab_t vvctre_gui_begin_tab;
static vvctre_gui_end_tab_t vvctre_gui_end_tab;
static vvctre_gui_checkbox_t vvctre_gui_checkbox;
static vvctre_get_cfg_module_t vvctre_get_cfg_module;
static vvctre_settings_set_console_model_t vvctre_settings_set_console_model;
static vvctre_settings_get_console_model_t vvctre_settings_get_console_model;
static vvctre_settings_write_config_savegame_t vvctre_settings_write_config_savegame;

static void* g_core = NULL;
static void* g_plugin_manager = NULL;
static void* g_cfg = NULL;

VVCTRE_PLUGIN_EXPORT void PluginLoaded(void* core, void* plugin_manager,
                                       void* required_functions[]) {
    g_core = core;
    g_plugin_manager = plugin_manager;

    vvctre_gui_begin_tab = (vvctre_gui_begin_tab_t)required_functions[0];
    vvctre_gui_end_tab = (vvctre_gui_end_tab_t)required_functions[1];
    vvctre_gui_checkbox = (vvctre_gui_checkbox_t)required_functions[2];
    vvctre_get_cfg_module = (vvctre_get_cfg_module_t)required_functions[3];
    vvctre_settings_set_console_model = (vvctre_settings_set_console_model_t)required_functions[4];
    vvctre_settings_get_console_model = (vvctre_settings_get_console_model_t)required_functions[5];
    vvctre_settings_write_config_savegame =
        (vvctre_settings_write_config_savegame_t)required_functions[6];
}

VVCTRE_PLUGIN_EXPORT void InitialSettingsOpening() {
    g_cfg = vvctre_get_cfg_module(g_core, g_plugin_manager);
}

VVCTRE_PLUGIN_EXPORT void AddTab() {
    if (vvctre_gui_begin_tab("2DS")) {
        bool is_2ds = vvctre_settings_get_console_model(g_cfg) == 3;
        if (vvctre_gui_checkbox("Is 2DS", &is_2ds)) {
            vvctre_settings_set_console_model(g_cfg, is_2ds ? 3 : 0);
            vvctre_settings_write_config_savegame(g_cfg);
        }
        vvctre_gui_end_tab();
    }
}
