#ifndef GAME_EVENTS_H_
#define GAME_EVENTS_H_

#define EVENT_UPDATE              (1 << 0U)    /* 0x00000000001 */
#define EVENT_CHANGE_MOVE_UP      (1 << 1U)    /* 0x00000000010 */
#define EVENT_CHANGE_MOVE_DOWN    (1 << 2U)    /* 0x00000000100 */
#define EVENT_CHANGE_MOVE_LEFT    (1 << 3U)    /* 0x00000001000 */
#define EVENT_CHANGE_MOVE_RIGHT   (1 << 4U)    /* 0x00000010000 */
#define EVENT_FOOD_COLLISION      (1 << 5U)    /* 0x00000100000 */
#define EVENT_HEAD_TAIL_COLLISION (1 << 6U)    /* 0x00001000000 */
#define EVENT_RESTART_GAME        (1 << 7U)    /* 0x00010000000 */
#define EVENT_PAUSE_GAME          (1 << 8U)    /* 0x00100000000 */
#define EVENT_TO_THE_MAIN_MENU    (1 << 9U)    /* 0x01000000000 */
#define EVENT_QUIT_GAME           (1 << 10U)   /* 0x10000000000 */

typedef unsigned int game_event_t;

#endif /* GAME_EVENTS_H_ */
