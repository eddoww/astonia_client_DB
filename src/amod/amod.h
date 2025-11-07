/*
 * Part of Astonia Client (c) Daniel Brockhaus. Please read license.txt.
 */

#include "../../src/platform.h"
#include "amod_structs.h"

void amod_init(void);
void amod_exit(void);
char *amod_version(void);
void amod_gamestart(void);
void amod_frame(void);
void amod_tick(void);
void amod_mouse_move(int x,int y);
void amod_update_hover_texts(void);

// the following functions should return 1 if they process the event and want the client
// and all later mods to ignore it.
// return -1 if you want the client to ignore it, but allow other mods to process it.
// return 0 otherwise
int amod_mouse_click(int x,int y,int what);
int amod_keydown(int key);  // if you catch keydown ...
int amod_keyup(int key);    // ... you must also catch keyup
int amod_client_cmd(char *buf);

// main mod only:
int amod_process(char *buf);    // return length of server command, 0 = unknown
int amod_prefetch(char *buf);   // return length of server command, 0 = unknown
int amod_display_skill_line(int v,int base,int curr,int cn,char *buf);
int amod_is_playersprite(int sprite);

// --------- Client exported functions -----------

// basics
IMPORT int note(const char *format,...) __attribute__((format(printf, 1, 2)));
IMPORT int warn(const char *format,...) __attribute__((format(printf, 1, 2)));
IMPORT int fail(const char *format,...) __attribute__((format(printf, 1, 2)));
IMPORT void paranoia(const char *format,...) __attribute__((format(printf, 1, 2)));
IMPORT void addline(const char *format,...) __attribute__((format(printf, 1, 2)));
// game, drawing
IMPORT void dd_push_clip(void);
IMPORT void dd_pop_clip(void);
IMPORT void dd_more_clip(int sx,int sy,int ex,int ey);
IMPORT void dd_copysprite(int sprite,int scrx,int scry,int light,int align);
IMPORT int dd_copysprite_fx(DDFX *ddfx,int scrx,int scry);
IMPORT void dd_rect(int sx,int sy,int ex,int ey,unsigned short int color);
IMPORT void dd_line(int fx,int fy,int tx,int ty,unsigned short col);
IMPORT int dd_textlength(int flags,const char *text);
IMPORT int dd_drawtext(int sx,int sy,unsigned short int color,int flags,const char *text);
IMPORT int dd_drawtext_break(int x,int y,int breakx,unsigned short color,int flags,const char *ptr);
IMPORT int dd_drawtext_break_length(int x,int y,int breakx,unsigned short color,int flags,const char *ptr);
IMPORT void dd_pixel(int x,int y,unsigned short col);
IMPORT int dd_drawtext_fmt(int sx,int sy,unsigned short int color,int flags,const char *format,...);
IMPORT int dd_drawtext_break_fmt(int sx,int sy,int breakx,unsigned short int color,int flags,const char *format,...);
IMPORT int dd_drawtext_nl(int x,int y,int unsigned short color,int flags,const char *ptr);
// gui, dots and buttons
IMPORT int dotx(int didx);
IMPORT int doty(int didx);
IMPORT int butx(int bidx);
IMPORT int buty(int bidx);
// gui helperls
IMPORT int get_near_ground(int x,int y);
IMPORT int get_near_item(int x,int y,int flag,int looksize);
IMPORT int get_near_char(int x,int y,int looksize);
IMPORT int mapmn(int x,int y);
// misc
IMPORT void set_teleport(int idx,int x,int y);
IMPORT int exp2level(int val);
IMPORT int level2exp(int level);
IMPORT int mil_rank(int exp);
// client / server communication
IMPORT void client_send(void *buf,int len);


// ---------- Client exported data structures -------------
IMPORT extern int skltab_cnt;
IMPORT extern struct skltab *skltab;
IMPORT extern int item_flags[];
IMPORT extern int weatab[];
IMPORT int act;
IMPORT int actx;
IMPORT int acty;
IMPORT extern int __yres;

IMPORT int vk_shift,vk_control,vk_alt;

IMPORT unsigned int cflags;        // current item flags
IMPORT unsigned int csprite;       // and sprite

IMPORT int originx;
IMPORT int originy;
IMPORT struct map map[MAPDX*MAPDY];
IMPORT struct map map2[MAPDX*MAPDY];

IMPORT int value[2][V_MAX];
IMPORT int item[INVENTORYSIZE];
IMPORT int item_flags[INVENTORYSIZE];
IMPORT int hp;
IMPORT int mana;
IMPORT int rage;
IMPORT int endurance;
IMPORT int lifeshield;
IMPORT int experience;
IMPORT int experience_used;
IMPORT int mil_exp;
IMPORT int gold;
IMPORT struct player player[MAXCHARS];
IMPORT union ceffect ceffect[MAXEF];
IMPORT unsigned char ueffect[MAXEF];
IMPORT int con_type;
IMPORT char con_name[80];
IMPORT int con_cnt;
IMPORT int container[CONTAINERSIZE];
IMPORT int price[CONTAINERSIZE];
IMPORT int itemprice[CONTAINERSIZE];
IMPORT int cprice;
IMPORT int lookinv[12];
IMPORT int looksprite,lookc1,lookc2,lookc3;
IMPORT char look_name[80];
IMPORT char look_desc[1024];
IMPORT char pent_str[7][80];
IMPORT int pspeed;
IMPORT int protocol_version;

IMPORT unsigned short int healthcolor,manacolor,endurancecolor,shieldcolor;
IMPORT unsigned short int whitecolor,lightgraycolor,graycolor,darkgraycolor,blackcolor;
IMPORT unsigned short int lightredcolor,redcolor,darkredcolor;
IMPORT unsigned short int lightgreencolor,greencolor,darkgreencolor;
IMPORT unsigned short int lightbluecolor,bluecolor,darkbluecolor;
IMPORT unsigned short int lightorangecolor,orangecolor,darkorangecolor;
IMPORT unsigned short int textcolor;

IMPORT struct quest quest[MAXQUEST];
IMPORT struct shrine_ppd shrine;

IMPORT char hover_bless_text[120];
IMPORT char hover_freeze_text[120];
IMPORT char hover_potion_text[120];
IMPORT char hover_rage_text[120];
IMPORT char hover_level_text[120];
IMPORT char hover_rank_text[120];
IMPORT char hover_time_text[120];

IMPORT int target_server;

IMPORT char password[16];
IMPORT char username[40];
IMPORT int tick;
IMPORT int mirror;
IMPORT int realtime;
IMPORT char server_url[256];
IMPORT int server_port;
IMPORT int want_width;
IMPORT int want_height;
IMPORT int sdl_scale;
IMPORT int sdl_frames;
IMPORT int sdl_multi;
IMPORT int sdl_cache_size;
IMPORT int frames_per_second;
IMPORT uint64_t game_options;
IMPORT int game_slowdown;


// ---------------- override-able functions, also exported from client ----------------
IMPORT int _is_cut_sprite(int sprite);
IMPORT int _is_mov_sprite(int sprite,int itemhint);
IMPORT int _is_door_sprite(int sprite);
IMPORT int _is_yadd_sprite(int sprite);
IMPORT int _get_chr_height(int csprite);
IMPORT int _trans_asprite(int mn,int sprite,int attick,unsigned char *pscale,unsigned char *pcr,unsigned char *pcg,unsigned char *pcb,unsigned char *plight,unsigned char *psat,unsigned short *pc1,unsigned short *pc2,unsigned short *pc3,unsigned short *pshine);
IMPORT int _trans_charno(int csprite,int *pscale,int *pcr,int *pcg,int *pcb,int *plight,int *psat,int *pc1,int *pc2,int *pc3,int *pshine,int attick);
IMPORT int _get_player_sprite(int nr,int zdir,int action,int step,int duration,int attick);
IMPORT void _trans_csprite(int mn,struct map *cmap,int attick);
IMPORT int _get_lay_sprite(int sprite,int lay);
IMPORT int _get_offset_sprite(int sprite,int *px,int *py);
IMPORT int _additional_sprite(int sprite,int attick);
IMPORT int _opt_sprite(int sprite);
IMPORT int _no_lighting_sprite(int sprite);
IMPORT int _get_skltab_sep(int i);
IMPORT int _get_skltab_index(int n);
IMPORT int _get_skltab_show(int i);
IMPORT int _do_display_random(void);
IMPORT int _do_display_help(int nr);

// ------------ declarations for functions the mod might provide -------------------
int is_cut_sprite(int sprite);
int is_mov_sprite(int sprite,int itemhint);
int is_door_sprite(int sprite);
int is_yadd_sprite(int sprite);
int get_chr_height(int csprite);
int trans_asprite(int mn,int sprite,int attick,unsigned char *pscale,unsigned char *pcr,unsigned char *pcg,unsigned char *pcb,unsigned char *plight,unsigned char *psat,unsigned short *pc1,unsigned short *pc2,unsigned short *pc3,unsigned short *pshine);
int trans_charno(int csprite,int *pscale,int *pcr,int *pcg,int *pcb,int *plight,int *psat,int *pc1,int *pc2,int *pc3,int *pshine,int attick);
int get_player_sprite(int nr,int zdir,int action,int step,int duration,int attick);
void trans_csprite(int mn,struct map *cmap,int attick);
int get_lay_sprite(int sprite,int lay);
int get_offset_sprite(int sprite,int *px,int *py);
int additional_sprite(int sprite,int attick);
int opt_sprite(int sprite);
int no_lighting_sprite(int sprite);
int get_skltab_sep(int i);
int get_skltab_index(int n);
int get_skltab_show(int i);
int do_display_random(void);
int do_display_help(int nr);


