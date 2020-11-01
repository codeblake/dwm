/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 10;       /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gaps      = 50;       /* gap size */
static const unsigned int gappih    = gaps;     /* horiz inner gap between windows */
static const unsigned int gappiv    = gaps;     /* vert inner gap between windows */
static const unsigned int gappoh    = gaps*2;   /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = gaps*2;   /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 40;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int startontag         = 1;        /* 0 means no tag active on start */
static const int zoomswap           = 1;        /* 0 means default swap behaviour, 1 will swap master with focus */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#689d6a";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray1, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact         = 0.465; /* factor of master area size [0.05..0.95] */
static const int nmaster         = 1;     /* number of clients in master area */
static const int resizehints     = 1;     /* 1 means respect size hints in tiled resizals */
static const int attachdirection = 4; /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* tiled clients*/
	{ "<*>",      NULL },    /* floating clients */
	{ "[M]",      monocle }, /* stacked maximised clients */
	{ NULL,       NULL },
};

/* key definitions */
#define SHIFT ShiftMask
#define CTRL ControlMask
#define ALT Mod1Mask
#define SUPER Mod4Mask

/* tag key bindings */
#define TAGKEYS(KEY,    TAG) \
	{ SUPER,            KEY,      view,           {.ui = 1 << TAG} }, \
	{ SUPER|CTRL,       KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ SUPER|SHIFT,      KEY,      tag,            {.ui = 1 << TAG} }, \
	{ SUPER|CTRL|SHIFT, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

/* key bindings */
static Key keys[] = {
	/* modifier         key           function        argument */
	{ CTRL,             XK_space,     spawn,          {.v = dmenucmd } },    // spawn dmenu
	{ SUPER,            XK_Return,    spawn,          {.v = termcmd } },     // spawn terminal
	{ SUPER,            XK_b,         togglebar,      {0} },                 // toggle bar
	{ SUPER,            XK_j,         focusstack,     {.i = -1 } },          // cycle focus clockwise
	{ SUPER,            XK_k,         focusstack,     {.i = +1 } },          // cycle focus anticlockwise
	{ SUPER,            XK_i,         incnmaster,     {.i = +1 } },          // cycle clients clockwise
	{ SUPER,            XK_d,         incnmaster,     {.i = -1 } },          // cycle clients anticlockwise
	{ SUPER,            XK_l,         setmfact,       {.f = -0.05} },        // resize left
	{ SUPER,            XK_semicolon, setmfact,       {.f = +0.05} },        // resize right
	{ SUPER,            XK_r,         quit,           {1} },                 // reload dwm
	{ SUPER,            XK_Escape,    quit,           {0} },                 // exit dwm

	/* gaps */
	{ SUPER,            XK_g,         togglegaps,     {0} },                 // toggle gaps
	{ SUPER|SHIFT,      XK_g,         defaultgaps,    {0} },                 // reset gaps
	{ SUPER|CTRL,       XK_g,         incrgaps,       {.i = +1 } },          // increase gaps
	{ SUPER|CTRL|SHIFT, XK_g,         incrgaps,       {.i = -1 } },          // decrease gaps
	/* outer gaps */
	{ SUPER|CTRL,       XK_o,         incrogaps,      {.i = +1 } },          // increase outer gaps
	{ SUPER|CTRL|SHIFT, XK_o,         incrogaps,      {.i = -1 } },          // decrease outer gaps
	/* inner gaps */
	{ SUPER|CTRL,       XK_i,         incrigaps,      {.i = +1 } },          // increase inner gaps
	{ SUPER|CTRL|SHIFT, XK_i,         incrigaps,      {.i = -1 } },          // decrease inner gaps
	/* inner gaps (slave) */
	{ SUPER|CTRL,       XK_s,         incrihgaps,     {.i = +1 } },          // increase inner slave gap
	{ SUPER|CTRL|SHIFT, XK_s,         incrihgaps,     {.i = -1 } },          // decrease inner slave gap
	/* inner gaps (master) */
	{ SUPER|CTRL,       XK_m,         incrivgaps,     {.i = +1 } },          // increase inner master gap
	{ SUPER|CTRL|SHIFT, XK_m,         incrivgaps,     {.i = -1 } },          // decrease inner master gap
	/* vertical gaps */
	{ SUPER|CTRL,       XK_v,         incrohgaps,     {.i = +1 } },          // increase vertical gap
	{ SUPER|CTRL|SHIFT, XK_v,         incrohgaps,     {.i = -1 } },          // decrease vertical gap
	/* horizontal gaps */
	{ SUPER|CTRL,       XK_h,         incrovgaps,     {.i = +1 } },          // increase horizontal gap
	{ SUPER|CTRL|SHIFT, XK_h,         incrovgaps,     {.i = -1 } },          // decrease horizontal gap

	/* client manipulation */
	{ SUPER,            XK_Tab,       zoom,           {0} },                 // swap master
	{ SUPER,            XK_BackSpace, view,           {0} },                 // previous client
	{ SUPER,            XK_q,         killclient,     {0} },                 // quit client

	/* layouts */
	{ SUPER,            XK_t,         setlayout,      {.v = &layouts[0]} },  // tile layout
	{ SUPER,            XK_f,         setlayout,      {.v = &layouts[1]} },  // float layout
	{ SUPER,            XK_m,         setlayout,      {.v = &layouts[2]} },  // monocle layout
	{ SUPER,            XK_space,     setlayout,      {0} },                 // toggle previous layout
	{ SUPER|CTRL,       XK_f,         togglefloating, {0} },                 // toggle floating on selected
	{ SUPER|SHIFT,      XK_f,         togglefullscr,  {0} },                 // toggle fullscreen
	{ SUPER,            XK_n,         cyclelayout,    {.i = -1 } },          // cycle layout left
	{ SUPER|SHIFT,      XK_n,         cyclelayout,    {.i = +1 } },          // cycle layout right

	/* tags */
	{ SUPER,            XK_a,         view,           {.ui = ~0 } },         // select all tags
	{ SUPER|SHIFT,      XK_a,         tag,            {.ui = ~0 } },         // add client to all tags

	/* monitors*/
	{ SUPER,            XK_comma,     focusmon,       {.i = -1 } },
	{ SUPER,            XK_period,    focusmon,       {.i = +1 } },
	{ SUPER|SHIFT,      XK_comma,     tagmon,         {.i = -1 } },
	{ SUPER|SHIFT,      XK_period,    tagmon,         {.i = +1 } },


	/* tag selection */
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         SUPER,          Button1,        movemouse,      {0} },
	{ ClkClientWin,         SUPER,          Button2,        togglefloating, {0} },
	{ ClkClientWin,         SUPER|SHIFT,    Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            SUPER,          Button1,        tag,            {0} },
	{ ClkTagBar,            SUPER,          Button3,        toggletag,      {0} },
};

