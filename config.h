/* See LICENSE file for copyright and license details. */

#include "X11/XF86keysym.h"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Source Code Pro:size=10" };
static const char dmenufont[]       = "Source Code Pro:size=10";
static const char col_gray0[]       = "#000000";
static const char col_gray1[]       = "#161616";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray0 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.7; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY ControlMask|ShiftMask
#define TAGKEYS(KEY,TAG) \
	{ ControlMask,                    KEY,      view,           {.ui = 1 << TAG} }, \
	{ Mod4Mask,                       KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ Mod4Mask|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ Mod4Mask|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *upvol[]    = { "amixer", "set", "Master", "2%+", NULL };
static const char *downvol[]  = { "amixer", "set", "Master", "2%-", NULL };
static const char *upbrightness[]   = { "x1_backlight_inc", NULL };
static const char *downbrightness[] = { "x1_backlight_dec", NULL };
static const char *scrotcmd[] = { "scrot", "-e", "mv $f ~/images/shots/", NULL };
static const char *scrotareacmd[] = { "scrot", "-s", "-e", "mv $f ~/images/shots/", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_o,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_t,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_y,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_n,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_m,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_v,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_h,      viewMove,       {.i = -1 } },
	{ MODKEY,                       XK_l,      viewMove,       {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|Mod1Mask,              XK_q,      quit,           {0} },
	{ 0,         XF86XK_AudioRaiseVolume,     spawn,           {.v = upvol } },
	{ 0,         XF86XK_AudioLowerVolume,     spawn,           {.v = downvol } },
	{ 0,             XF86XK_AudioMicMute,     spawn,           {.v = upbrightness } },
	{ 0,                XF86XK_AudioMute,     spawn,           {.v = downbrightness } },
	{ 0,                          0xff61,     spawn,           {.v = scrotcmd } },
	{ ShiftMask,                  0xff61,     spawn,           {.v = scrotareacmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         Mod4Mask,       Button1,        movemouse,      {0} },
	{ ClkClientWin,         Mod4Mask,       Button2,        togglefloating, {0} },
	{ ClkClientWin,         Mod4Mask,       Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

