#ifndef GAME_EVENTS_H_
#define GAME_EVENTS_H_

#define EVENT_UPDATE              (1 << 0U)    /* 0x000000000001 */
#define EVENT_CHANGE_MOVE_UP      (1 << 1U)    /* 0x000000000010 */
#define EVENT_CHANGE_MOVE_DOWN    (1 << 2U)    /* 0x000000000100 */
#define EVENT_CHANGE_MOVE_LEFT    (1 << 3U)    /* 0x000000001000 */
#define EVENT_CHANGE_MOVE_RIGHT   (1 << 4U)    /* 0x000000010000 */
#define EVENT_FOOD_COLLISION      (1 << 5U)    /* 0x000000100000 */
#define EVENT_HEAD_TAIL_COLLISION (1 << 6U)    /* 0x000001000000 */
#define EVENT_RESTART_GAME        (1 << 9U)    /* 0x001000000000 */
#define EVENT_PAUSE_GAME          (1 << 10U)   /* 0x010000000000 */
#define EVENT_TO_THE_MAIN_MENU    (1 << 11U)   /* 0x100000000000 */

typedef unsigned int game_event_t;

#endif /* GAME_EVENTS_H_ */
