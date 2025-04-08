#ifndef PLAYERS_H_
#define PLAYERS_H_

#include "../lib/cJSON/cJSON.h"

#define PLAYERS_STATS_FILENAME "players.json"
#define PLAYERS                8

typedef struct PlayerTime {
    int hours;
    int mins;
    int secs;
} player_time_t;

typedef struct Player {
    bool was_used;
    int id;
    char name[256];
    player_time_t game_play_time;
    int tail_lenght;
    unsigned long score;
} player_t;

void create_players(const char* filename, int num_of_players);
void write_players(player_t* players, const char* filename);
void draw_player_play_time(Vector2 pos);
player_time_t get_player_play_time(void);
void draw_player_board(player_t* players);
player_t cjson_object2player(cJSON* player_conf);
player_t* load_players_data(const char* filename);
player_t get_player_from_id(int id, const char* filename);
player_t get_player_from_name(const char* name, const char* filename);
void save_player_data(const char* filename, player_t new_data);

#endif
