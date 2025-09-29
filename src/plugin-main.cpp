/*
OBS Starter Plugin
Copyright (C) 2024

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <plugin-support.h>
#include <QMainWindow>
#include <QAction>
#include <QDialog>
#include <vector>
#include <util/platform.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#endif

#include "config-dialog.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

static ConfigDialog *config_dialog = nullptr;

#ifdef _WIN32
static std::vector<PROCESS_INFORMATION> running_processes;
#else
static std::vector<pid_t> running_processes;
#endif

static std::vector<ExecutableConfig> executable_configs;

static void start_executables()
{
    // Clear any previous process handles
    running_processes.clear();
    
    for (const auto &config : executable_configs) {
        if (!config.path.empty()) {
#ifdef _WIN32
            STARTUPINFOA si = {};
            PROCESS_INFORMATION pi = {};
            si.cb = sizeof(si);
            
            // Set window state based on minimize option
            if (config.start_minimized) {
                si.dwFlags = STARTF_USESHOWWINDOW;
                si.wShowWindow = SW_MINIMIZE;
            }
            
            // Initialize to invalid values
            pi.hProcess = INVALID_HANDLE_VALUE;
            pi.hThread = INVALID_HANDLE_VALUE;
            
            if (CreateProcessA(nullptr, (LPSTR)config.path.c_str(), nullptr, nullptr, 
                             FALSE, 0, nullptr, nullptr, &si, &pi)) {
                running_processes.push_back(pi);
                obs_log(LOG_INFO, "Started executable%s: %s", 
                       config.start_minimized ? " (minimized)" : "", config.path.c_str());
            } else {
                obs_log(LOG_WARNING, "Failed to start executable: %s", config.path.c_str());
            }
#else
            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                // Note: Minimizing on Linux/macOS would require window manager specific commands
                // This is a basic implementation that starts the process normally
                execl(config.path.c_str(), config.path.c_str(), (char *)nullptr);
                exit(1);
            } else if (pid > 0) {
                // Parent process
                running_processes.push_back(pid);
                obs_log(LOG_INFO, "Started executable%s: %s", 
                       config.start_minimized ? " (minimize requested)" : "", config.path.c_str());
            } else {
                obs_log(LOG_WARNING, "Failed to start executable: %s", config.path.c_str());
            }
#endif
        }
    }
}

static void stop_executables()
{
    // Create a safe copy of the size to avoid accessing potentially corrupted vectors
    size_t process_count = running_processes.size();
    size_t config_count = executable_configs.size();
    
    obs_log(LOG_INFO, "Stopping %zu processes...", process_count);
    
    for (size_t i = 0; i < process_count; ++i) {
        // Safety check: only proceed if we have valid config
        bool should_shutdown = (i < config_count) ? executable_configs[i].shutdown_enabled : true;
        
        if (should_shutdown) {
#ifdef _WIN32
            if (running_processes[i].hProcess != INVALID_HANDLE_VALUE) {
                try {
                    TerminateProcess(running_processes[i].hProcess, 0);
                    WaitForSingleObject(running_processes[i].hProcess, 1000); // Reduced timeout for shutdown
                    CloseHandle(running_processes[i].hProcess);
                    CloseHandle(running_processes[i].hThread);
                    
                    // Safe logging without accessing potentially corrupted config
                    obs_log(LOG_INFO, "Stopped executable at index %zu", i);
                } catch (...) {
                    obs_log(LOG_ERROR, "Exception while stopping process %zu", i);
                }
            }
#else
            if (running_processes[i] > 0) {
                try {
                    kill(running_processes[i], SIGTERM);
                    usleep(500000); // 0.5 second wait
                    kill(running_processes[i], SIGKILL);
                    obs_log(LOG_INFO, "Stopped executable at index %zu", i);
                } catch (...) {
                    obs_log(LOG_ERROR, "Exception while stopping process %zu", i);
                }
            }
#endif
        }
    }
    
    // Clear the vectors safely
    try {
        running_processes.clear();
        obs_log(LOG_INFO, "Cleared process list");
    } catch (...) {
        obs_log(LOG_ERROR, "Exception while clearing process list");
    }
}

static void load_settings()
{
    char *config_path = obs_module_get_config_path(obs_current_module(), "config.json");
    if (!config_path)
        return;
        
    obs_data_t *data = obs_data_create_from_json_file(config_path);
    if (!data) {
        bfree(config_path);
        return;
    }
        
    obs_data_array_t *array = obs_data_get_array(data, "executables");
    if (!array) {
        obs_data_release(data);
        bfree(config_path);
        return;
    }
    
    executable_configs.clear();
    size_t count = obs_data_array_count(array);
    
    for (size_t i = 0; i < count; i++) {
        obs_data_t *item = obs_data_array_item(array, i);
        if (item) {
            ExecutableConfig config;
            config.path = obs_data_get_string(item, "path");
            config.shutdown_enabled = obs_data_get_bool(item, "shutdown_enabled");
            config.start_minimized = obs_data_get_bool(item, "start_minimized");
            executable_configs.push_back(config);
            obs_data_release(item);
        }
    }
    
    obs_data_array_release(array);
    obs_data_release(data);
    bfree(config_path);
}

static void save_settings()
{
    char *config_path = obs_module_get_config_path(obs_current_module(), "config.json");
    if (!config_path)
        return;
    
    // Ensure the directory exists
    char *config_dir = obs_module_get_config_path(obs_current_module(), "");
    if (config_dir) {
#ifdef _WIN32
        _mkdir(config_dir);
#else
        mkdir(config_dir, 0755);
#endif
        bfree(config_dir);
    }
        
    obs_data_t *data = obs_data_create();
    obs_data_array_t *array = obs_data_array_create();
    
    for (const auto &config : executable_configs) {
        obs_data_t *item = obs_data_create();
        obs_data_set_string(item, "path", config.path.c_str());
        obs_data_set_bool(item, "shutdown_enabled", config.shutdown_enabled);
        obs_data_set_bool(item, "start_minimized", config.start_minimized);
        obs_data_array_push_back(array, item);
        obs_data_release(item);
    }
    
    obs_data_set_array(data, "executables", array);
    obs_data_array_release(array);
    
    if (!obs_data_save_json_safe(data, config_path, "tmp", "bak")) {
        obs_log(LOG_WARNING, "Failed to save configuration to %s", config_path);
    } else {
        obs_log(LOG_INFO, "Configuration saved successfully to %s", config_path);
    }
    
    obs_data_release(data);
    bfree(config_path);
}

// Implementation of functions declared in plugin-support.h
std::vector<ExecutableConfig> get_executable_configs()
{
    return executable_configs;
}

void update_executable_configs(const std::vector<ExecutableConfig> &configs)
{
    executable_configs = configs;
    save_settings();
}

static void on_frontend_event(enum obs_frontend_event event, void *private_data)
{
    switch (event) {
    case OBS_FRONTEND_EVENT_FINISHED_LOADING:
        start_executables();
        break;
    case OBS_FRONTEND_EVENT_EXIT:
        stop_executables();
        break;
    default:
        break;
    }
}

static void show_config_dialog()
{
    // Safety check - don't create dialogs during shutdown or if no main window
    QMainWindow *main_window = (QMainWindow *)obs_frontend_get_main_window();
    if (!main_window) {
        obs_log(LOG_WARNING, "Cannot show config dialog - no main window available");
        return;
    }
    
    if (!config_dialog) {
        try {
            config_dialog = new ConfigDialog(main_window);
            obs_log(LOG_INFO, "Created new config dialog");
        } catch (...) {
            obs_log(LOG_ERROR, "Failed to create config dialog");
            return;
        }
    }
    
    if (config_dialog) {
        config_dialog->show();
        config_dialog->raise();
        config_dialog->activateWindow();
        obs_log(LOG_INFO, "Config dialog shown");
    }
}

bool obs_module_load(void)
{
    obs_log(LOG_INFO, "OBS Starter plugin loaded successfully (version %s)", PLUGIN_VERSION);
    
    // Load settings
    load_settings();
    
    // Register frontend events
    obs_frontend_add_event_callback(on_frontend_event, nullptr);
    
    // Add menu item
    QMainWindow *main_window = (QMainWindow *)obs_frontend_get_main_window();
    if (main_window) {
        QAction *action = (QAction *)obs_frontend_add_tools_menu_qaction("OBS Starter Config");
        QObject::connect(action, &QAction::triggered, show_config_dialog);
    }
    
    return true;
}

void obs_module_unload(void)
{
    obs_log(LOG_INFO, "OBS Starter plugin starting unload...");
    
    // Stop executables first - this should be safe
    try {
        stop_executables();
        obs_log(LOG_INFO, "Stopped executables successfully");
    } catch (...) {
        obs_log(LOG_ERROR, "Exception while stopping executables");
    }
    
    // Don't touch Qt objects at all during shutdown
    // Just set the pointer to nullptr without any Qt calls
    config_dialog = nullptr;
    
    obs_log(LOG_INFO, "OBS Starter plugin unloaded successfully");
}