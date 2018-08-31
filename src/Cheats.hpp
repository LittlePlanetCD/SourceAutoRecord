#pragma once
#include "Variable.hpp"

extern Variable sar_autorecord;
extern Variable sar_save_flag;
extern Variable sar_time_demo_dev;
extern Variable sar_sum_during_session;
extern Variable sar_timer_always_running;
extern Variable sar_hud_text;
extern Variable sar_hud_position;
extern Variable sar_hud_angles;
extern Variable sar_hud_velocity;
extern Variable sar_hud_session;
extern Variable sar_hud_last_session;
extern Variable sar_hud_sum;
extern Variable sar_hud_timer;
extern Variable sar_hud_avg;
extern Variable sar_hud_cps;
extern Variable sar_hud_demo;
extern Variable sar_hud_jumps;
extern Variable sar_hud_portals;
extern Variable sar_hud_steps;
extern Variable sar_hud_jump;
extern Variable sar_hud_jump_peak;
extern Variable sar_hud_trace;
extern Variable sar_hud_frame;
extern Variable sar_hud_last_frame;
extern Variable sar_hud_velocity_peak;
extern Variable sar_hud_default_spacing;
extern Variable sar_hud_default_padding_x;
extern Variable sar_hud_default_padding_y;
extern Variable sar_hud_default_font_index;
extern Variable sar_hud_default_font_color;
extern Variable sar_ihud;
extern Variable sar_ihud_x;
extern Variable sar_ihud_y;
extern Variable sar_ihud_button_padding;
extern Variable sar_ihud_button_size;
extern Variable sar_ihud_button_color;
extern Variable sar_ihud_font_color;
extern Variable sar_ihud_font_index;
extern Variable sar_ihud_layout;
extern Variable sar_ihud_shadow;
extern Variable sar_ihud_shadow_color;
extern Variable sar_ihud_shadow_font_color;
extern Variable sar_sr_hud;
extern Variable sar_sr_hud_x;
extern Variable sar_sr_hud_y;
extern Variable sar_sr_hud_font_color;
extern Variable sar_sr_hud_font_index;
extern Variable sar_stats_jumps_xy;
extern Variable sar_stats_velocity_peak_xy;
extern Variable sar_stats_auto_reset;
extern Variable sar_autojump;
extern Variable sar_jumpboost;
extern Variable sar_aircontrol;
extern Variable sar_disable_challenge_stats_hud;
extern Variable sar_tas_autostart;
extern Variable sar_tas_autorecord;
extern Variable sar_tas_autoplay;
extern Variable sar_debug_event_queue;
extern Variable sar_debug_game_events;
extern Variable sar_speedrun_autostart;
extern Variable sar_speedrun_autostop;

extern Variable cl_showpos;
extern Variable sv_cheats;
extern Variable sv_footsteps;
extern Variable sv_alternateticks;
extern Variable sv_bonus_challenge;
extern Variable sv_accelerate;
extern Variable sv_airaccelerate;
extern Variable sv_friction;
extern Variable sv_maxspeed;
extern Variable sv_stopspeed;
extern Variable sv_maxvelocity;
extern Variable sv_transition_fade_time;
extern Variable sv_laser_cube_autoaim;
extern Variable ui_loadingscreen_transition_time;
extern Variable hide_gun_when_holding;

class Cheats {
public:
    void Init();
    void Shutdown();
};

extern Cheats* cheats;
