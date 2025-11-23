/**
 * @file lv_conf.h
 * Configuration file for LVGL v8.3.11
 * Optimized for WeAct 2.13" EPD with SSD1680 controller
 * STM32WB55 target - Low power, monochrome display
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/

/*Color depth: 1 (1 byte per pixel) for monochrome EPD*/
#define LV_COLOR_DEPTH 1

/*Swap the 2 bytes of RGB565 color. Not needed for monochrome*/
#define LV_COLOR_16_SWAP 0

/*Disable transparent background for EPD*/
#define LV_COLOR_SCREEN_TRANSP 0

/*=========================
   MEMORY SETTINGS
 *=========================*/

/*Use custom malloc/free for better memory management*/
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
    /*Optimized memory size for STM32WB55 - 16KB for LVGL*/
    #define LV_MEM_SIZE (16U * 1024U)
    
    /*No fixed memory address*/
    #define LV_MEM_ADR 0
#endif

/*Number of intermediate memory buffers - reduced for EPD*/
#define LV_MEM_BUF_MAX_NUM 4

/*Use standard memcpy/memset*/
#define LV_MEMCPY_MEMSET_STD 0

/*====================
   HAL SETTINGS
 *====================*/

/*Slow refresh period for EPD (500ms)*/
#define LV_DISP_DEF_REFR_PERIOD 500

/*Input device read period*/
#define LV_INDEV_DEF_READ_PERIOD 100

/*Custom tick source using HAL*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE "main.h"
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (HAL_GetTick())
#endif

/*DPI setting for EPD*/
#define LV_DPI_DEF 130

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

/*-------------
 * Drawing
 *-----------*/

/*Disable complex draw engine for EPD*/
#define LV_DRAW_COMPLEX 0

/*Simple layer buffer sizes - reduced for EPD*/
#define LV_LAYER_SIMPLE_BUF_SIZE (4 * 1024)
#define LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE (2 * 1024)

/*Disable image caching for EPD*/
#define LV_IMG_CACHE_DEF_SIZE 0

/*Minimal gradient support*/
#define LV_GRADIENT_MAX_STOPS 2
#define LV_GRAD_CACHE_DEF_SIZE 0

/*Disable dithering for monochrome*/
#define LV_DITHER_GRADIENT 0

/*Rotation buffer size*/
#define LV_DISP_ROT_MAX_BUF (4*1024)

/*-------------
 * GPU
 *-----------*/

/*No GPU acceleration for EPD*/
#define LV_USE_GPU_ARM2D 0
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_GPU_RA6M3_G2D 0
#define LV_USE_GPU_SWM341_DMA2D 0
#define LV_USE_GPU_NXP_PXP 0
#define LV_USE_GPU_NXP_VG_LITE 0
#define LV_USE_GPU_SDL 0

/*-------------
 * Logging
 *-----------*/

/*Enable minimal logging for debugging*/
#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 0
    #define LV_LOG_TRACE_MEM 0
    #define LV_LOG_TRACE_TIMER 0
    #define LV_LOG_TRACE_INDEV 0
    #define LV_LOG_TRACE_DISP_REFR 1
    #define LV_LOG_TRACE_EVENT 0
    #define LV_LOG_TRACE_OBJ_CREATE 0
    #define LV_LOG_TRACE_LAYOUT 0
    #define LV_LOG_TRACE_ANIM 0
#endif

/*-------------
 * Asserts
 *-----------*/

/*Essential asserts only*/
#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MALLOC 1
#define LV_USE_ASSERT_STYLE 0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ 0

/*-------------
 * Others
 *-----------*/

/*Enable performance monitoring for optimization*/
#define LV_USE_PERF_MONITOR 1
#if LV_USE_PERF_MONITOR
    #define LV_USE_PERF_MONITOR_POS LV_ALIGN_TOP_RIGHT
#endif

/*Enable memory monitoring*/
#define LV_USE_MEM_MONITOR 1
#if LV_USE_MEM_MONITOR
    #define LV_USE_MEM_MONITOR_POS LV_ALIGN_TOP_LEFT
#endif

/*Disable refresh debug*/
#define LV_USE_REFR_DEBUG 0

/*Custom sprintf*/
#define LV_SPRINTF_CUSTOM 0
#define LV_USE_USER_DATA 1
#define LV_ENABLE_GC 0

/*=====================
 *  COMPILER SETTINGS
 *====================*/

#define LV_BIG_ENDIAN_SYSTEM 0
#define LV_ATTRIBUTE_TICK_INC
#define LV_ATTRIBUTE_TIMER_HANDLER
#define LV_ATTRIBUTE_FLUSH_READY
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1
#define LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_LARGE_CONST
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY
#define LV_ATTRIBUTE_FAST_MEM
#define LV_ATTRIBUTE_DMA
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning

/*Use 16-bit coordinates for small EPD*/
#define LV_USE_LARGE_COORD 0

/*==================
 *   FONT USAGE
 *===================*/

/*Essential Montserrat fonts for English/French*/
#define LV_FONT_MONTSERRAT_8  1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1

/*Disable unused font sizes*/
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/*Special font features*/
#define LV_FONT_MONTSERRAT_12_SUBPX 0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0

/*Chinese CJK font support*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_16_CJK 1

/*Monospace fonts*/
#define LV_FONT_UNSCII_8  1
#define LV_FONT_UNSCII_16 0

/*Custom fonts*/
#define LV_FONT_CUSTOM_DECLARE

/*Default font*/
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*Font format settings*/
#define LV_FONT_FMT_TXT_LARGE 1
#define LV_USE_FONT_COMPRESSED 0
#define LV_USE_FONT_SUBPX 0
#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 *  TEXT SETTINGS
 *=================*/

/*UTF-8 encoding for international support*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*Text break characters*/
#define LV_TXT_BREAK_CHARS " ,.;:-_"

/*Line break settings*/
#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/*Color command character*/
#define LV_TXT_COLOR_CMD "#"

/*Bidirectional text support*/
#define LV_USE_BIDI 0

/*Arabic/Persian character processing*/
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 *  WIDGET USAGE
 *================*/

/*Essential widgets for EPD UI*/
#define LV_USE_ARC        1
#define LV_USE_BAR        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_CANVAS     1
#define LV_USE_CHECKBOX   1
#define LV_USE_DROPDOWN   1
#define LV_USE_IMG        1
#define LV_USE_LABEL      1
#define LV_USE_LINE       1
#define LV_USE_ROLLER     1
#define LV_USE_SLIDER     1
#define LV_USE_SWITCH     1
#define LV_USE_TEXTAREA   1
#define LV_USE_TABLE      1

/*Label settings*/
#if LV_USE_LABEL
    #define LV_LABEL_TEXT_SELECTION 0
    #define LV_LABEL_LONG_TXT_HINT 1
#endif

/*Roller settings*/
#if LV_USE_ROLLER
    #define LV_ROLLER_INF_PAGES 3
#endif

/*Textarea settings*/
#if LV_USE_TEXTAREA != 0
    #define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500
#endif

/*==================
 * EXTRA COMPONENTS
 *==================*/

/*-----------
 * Widgets
 *----------*/
#define LV_USE_ANIMIMG    0
#define LV_USE_CALENDAR   0
#define LV_USE_CHART      0
#define LV_USE_COLORWHEEL 0
#define LV_USE_IMGBTN     1
#define LV_USE_KEYBOARD   0
#define LV_USE_LED        1
#define LV_USE_LIST       1
#define LV_USE_MENU       1
#define LV_USE_METER      0
#define LV_USE_MSGBOX     1
#define LV_USE_SPAN       1
#define LV_USE_SPINBOX    1
#define LV_USE_SPINNER    0
#define LV_USE_TABVIEW    1
#define LV_USE_TILEVIEW   0
#define LV_USE_WIN        1

/*Span settings*/
#if LV_USE_SPAN
    #define LV_SPAN_SNIPPET_STACK_SIZE 32
#endif

/*-----------
 * Themes
 *----------*/

/*Use default theme optimized for monochrome*/
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
    #define LV_THEME_DEFAULT_DARK 0
    #define LV_THEME_DEFAULT_GROW 0
    #define LV_THEME_DEFAULT_TRANSITION_TIME 0
#endif

#define LV_USE_THEME_BASIC 1
#define LV_USE_THEME_MONO 1

/*-----------
 * Layouts
 *----------*/
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/*---------------------
 * 3rd party libraries
 *--------------------*/

/*File systems - disabled for low power*/
#define LV_USE_FS_STDIO 0
#define LV_USE_FS_POSIX 0
#define LV_USE_FS_WIN32 0
#define LV_USE_FS_FATFS 0
#define LV_USE_FS_LITTLEFS 0

/*Image decoders - disabled for memory*/
#define LV_USE_PNG 0
#define LV_USE_BMP 0
#define LV_USE_SJPG 0
#define LV_USE_GIF 0
#define LV_USE_QRCODE 0
#define LV_USE_FREETYPE 0
#define LV_USE_TINY_TTF 0
#define LV_USE_RLOTTIE 0
#define LV_USE_FFMPEG 0

/*-----------
 * Others
 *----------*/
#define LV_USE_SNAPSHOT 0
#define LV_USE_MONKEY 0
#define LV_USE_GRIDNAV 0
#define LV_USE_FRAGMENT 0
#define LV_USE_IMGFONT 0
#define LV_USE_MSG 0
#define LV_USE_IME_PINYIN 0

/*==================
 * EXAMPLES
 *==================*/
#define LV_BUILD_EXAMPLES 0

/*===================
 * DEMO USAGE
 ====================*/
#define LV_USE_DEMO_WIDGETS 0
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0
#define LV_USE_DEMO_BENCHMARK 0
#define LV_USE_DEMO_STRESS 0
#define LV_USE_DEMO_MUSIC 0

#endif /*LV_CONF_H*/
