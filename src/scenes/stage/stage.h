#ifndef PSXF_GUARD_STAGE_H
#define PSXF_GUARD_STAGE_H

#include "../../psx/io.h"
#include "../../psx/gfx.h"
#include "../../psx/pad.h"

#include "../../psx/fixed.h"
#include "character.h"
#include "player.h"
#include "object.h"
#include "pause.h"
#include "../../fonts/font.h"

#define STAGE_FLAG_JUST_STEP     (1 << 0) //Song just stepped this frame
#define STAGE_FLAG_VOCAL_ACTIVE  (1 << 1) //Song's vocal track is currently active
#define STAGE_FLAG_SCORE_REFRESH (1 << 2) //Score text should be refreshed

#define STAGE_LOAD_PLAYER     (1 << 0) //Reload player character
#define STAGE_LOAD_OPPONENT   (1 << 1) //Reload opponent character
#define STAGE_LOAD_GIRLFRIEND (1 << 2) //Reload girlfriend character
#define STAGE_LOAD_STAGE      (1 << 3) //Reload stage
#define STAGE_LOAD_FLAG       (1 << 7)

//Stage enums
typedef enum
{
	StageId_1_1,
	StageId_1_2,
	StageId_1_3,
	StageId_1_4,
	StageId_1_5,
	
	StageId_Max
} StageId;

#define StageId_LastVanilla StageId_1_5

typedef enum
{
	StageDiff_Easy,
	StageDiff_Normal,
	StageDiff_Hard,
} StageDiff;

typedef enum
{
	StageMode_Normal,
	StageMode_Swap,
	StageMode_2P,
} StageMode;

typedef enum
{
	StageTrans_Menu,
	StageTrans_NextSong,
	StageTrans_Reload,
} StageTrans;

//Stage background
typedef struct StageBack
{
	//Stage background functions
	void (*draw_fg)(struct StageBack*);
	void (*draw_md)(struct StageBack*);
	void (*draw_bg)(struct StageBack*);
	void (*free)(struct StageBack*);
} StageBack;

//Stage definitions
typedef struct
{
	//Characters
	struct
	{
		const char *path;
		fixed_t x, y, scale;
	} pchar, ochar, gchar;
	
	//Characters
	u8 death_offset;
	
	//Stage background
	StageBack* (*back)();
	
	//Camera Offsets
	fixed_t offset_x, offset_y, offset_zoom;
	
	//Song info
	u8 week, week_song;
	
	//Mus file
	u8 music_track, music_channel;
	const char credits[40];
	
	StageId next_stage;
	u8 next_load;
} StageDef;

//Stage state
#define SECTION_FLAG_OPPFOCUS (1 << 15) //Focus on opponent
#define SECTION_FLAG_BPM_MASK 0x7FFF //1/24

typedef struct
{
	u16 end; //1/12 steps
	u16 flag;
} Section;

#define NOTE_FLAG_OPPONENT    (1 << 2) //Note is opponent's
#define NOTE_FLAG_SUSTAIN     (1 << 3) //Note is a sustain note
#define NOTE_FLAG_SUSTAIN_END (1 << 4) //Is either end of sustain
#define NOTE_FLAG_ALT_ANIM    (1 << 5) //Note plays alt animation
#define NOTE_FLAG_MINE        (1 << 6) //Note is a mine
#define NOTE_FLAG_BULLET      (1 << 7) //Note is a bullet
#define NOTE_FLAG_HIT         (1 << 8) //Note has been hit

typedef struct
{
	u16 pos; //1/12 steps
	u16 type;
} Note;

typedef struct
{
	Character *character;
	
	fixed_t arrow_hitan[4]; //Arrow hit animation for presses
	
	s16 health;
	u16 combo;
	
	boolean refresh_miss;
	s32 miss;
	u16 antispam;
	
	boolean refresh_accuracy;
	s32 min_accuracy;
	u8 accuracy;
	s32 max_accuracy;
	
	boolean refresh_score;
	int score; 
	u32 max_score;
	char score_text[50];
	
	u16 pad_held, pad_press;
} PlayerState;

typedef struct
{
	DISPENV disp[2];
	DRAWENV draw[2];
	
	//Stage settings
	int pause_state;
	int pal_i;
	struct
	{
		//Song Scores
		int savescore[StageId_Max][3];
		
		//Controls settings
		u16 control_keys[4];
		
		//Adjust Combo settings
		s16 combox, comboy;
		
		//Graphics settings
		boolean lowgraphics, palmode;
		
		//Visuals & UI settings
		boolean notesplashes, hidehud, timebar, flashinglights, camerazooms, combostack;
		
		//Gameplay settings
		s32 mode;
		boolean downscroll, middlescroll, opponentnotes;
		boolean ghost;
		
		//Memory Card settings
		boolean autosave;
		
		boolean botplay, expsync;
	} prefs;
	
	//Debug Variables
	boolean debug, freecam;
	
	u32 offset;
	u32 note_x[8];
	u32 note_y[8];
	
	fixed_t pause_scroll;
	u8 pause_select;
	boolean paused;
	
	//HUD textures
	Gfx_Tex tex_hud0, tex_icons, tex_intro;
	
	//Stage data
	const StageDef *stage_def;
	StageId stage_id;
	StageDiff stage_diff;
	char credits[40];
	
	IO_Data chart_data;
	Section *sections;
	Note *notes;
	size_t num_notes;
	
	fixed_t speed;
	fixed_t step_crochet, step_time;
	fixed_t early_safe, late_safe, early_sus_safe, late_sus_safe;
	
	//Stage state
	boolean story;
	u8 flag;
	StageTrans trans;
	
	struct
	{
		// Specs
		boolean force;
		fixed_t speed;
		
		// Positions
		fixed_t x, y, zoom, bzoom, angle, hudangle;
		
		struct
		{
			fixed_t x, y, zoom;
		} offset;
		
		// Targets
		fixed_t tx, ty, tz;
		s16 ta, hudta;
	} camera;
	fixed_t bump, sbump;
	
	StageBack *back;
	
	Character *player;
	Character *opponent;
	Character *gf;
	
	boolean hidegf;
	
	Section *cur_section; //Current section
	Note *cur_note; //First visible and hittable note, used for drawing and hit detection
	
	fixed_t note_scroll, song_time, interp_time, interp_ms, interp_speed;
	
	u16 last_bpm;
	
	fixed_t time_base;
	u16 step_base;
	Section *section_base;
	
	s32 song_step;
	
	u8 gf_speed; //Typically 4 steps, changes in Fresh
	
	PlayerState player_state[2];
	s32 max_score;
	
	enum
	{
		StageState_Play, //Game is playing as normal
		StageState_Dead,       //Start BREAK animation and reading extra data from CD
		StageState_DeadLoad,   //Wait for said data to be read
		StageState_DeadDrop,   //Mic drop
		StageState_DeadRetry,  //Retry prompt
		StageState_DeadDecide, //Decided
	} state;
	
	u8 note_swap;
	
	//Object lists
	ObjectList objlist_splash, objlist_fg, objlist_bg;
	
	//Intro
	u8 frame;
	boolean introsound;
} Stage;

extern Stage stage;

//Stage drawing functions
void Stage_DrawRect(const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb);
void Stage_BlendRect(const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb, int mode);
void Stage_DrawTexRotateCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, u8 angle, fixed_t hx, fixed_t hy, u8 r, u8 g, u8 b, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexRotate(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, u8 angle, fixed_t hx, fixed_t hy, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 r, u8 g, u8 b);
void Stage_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, u8 r, u8 g, u8 b, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation);
void Stage_BlendTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation, u8 r, u8 g, u8 b, u8 mode);
void Stage_BlendTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation, u8 mode);
void Stage_BlendTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 mode);

//Stage functions
void Stage_Load(StageId id, StageDiff difficulty, boolean story);
void Stage_Unload();
void Stage_Tick();

#endif
