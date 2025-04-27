/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * separator           string to echo                  NULL
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 *                                                     NULL on OpenBSD
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */
// static const struct arg args[] = {
//     // 网络速度
//     { netspeed_rx,   "⬇ %s ",      "wlp4s0" },  // 下载速度
//     { netspeed_tx,   "⬆ %s |  ",   "wlp4s0" },  // 上传速度
//
//     // CPU 信息
//     { cpu_perc,      "⚙ CPU:%s%%",      NULL },  // CPU 使用率
//     { cpu_freq,      "-%sHz |  ",             NULL },  // CPU 频率
//     { run_command,   "ἲ1 %s |  ",         "/home/ming/scripts/check_cpu_temp.sh" }, // CPU 温度
//
//     // 内存信息
//     { ram_perc,      "ὋB RAM:%s%%",      NULL },  // 内存使用率
//     { ram_free,      "-%s |  ",                NULL },  // 空闲内存
//
//     // 交换分区 (Swap)
//     { swap_perc,     "♻ S:%s%%  |  ",     NULL },  // 交换分区使用率
//
//     // 磁盘信息
//     { disk_free,     "ὋE D:%s",         "/" },   // 根目录剩余空间
//     { disk_free,     "-%s |  ",                "/home" },  // /home 目录剩余空间
//
//     // 音量和亮度
//     { run_command,   "🎧 V:%s |  ",    "/home/ming/scripts/check_vol.sh" }, // 音量
//     { run_command,   "🔦 L:%s |  ",    "light" },                         // 亮度
//
//     // 电池信息
//     { battery_perc,  "🔋 B:%s%%",     "BAT1" },  // 电池电量
//     { run_command,   "-%s/",                   "/home/ming/scripts/battery_rate.sh" }, // 电池充电速率
//     { battery_state, "%s |  ",                 "BAT1" }, // 电池状态 (充电/放电)
//
//     // 日期与时间
//     { datetime,      "🕒 %s",        "%F %T" }  // 日期 & 时间
// };
static const struct arg args[] = {
    { netspeed_rx,   "↓:%s ",      "wlp4s0" },  // 替换 "wlan0" 为你的接口名
    { netspeed_tx,   "↑:%s | ",  "wlp4s0" },  // 替换 "wlan0" 为你的接口名
	{ cpu_perc,		"CPU:%s%%",		NULL },
	{ cpu_freq,		"-%sHz",		NULL },
	{ run_command,		"-%s | ",		"/home/ming/scripts/check_cpu_temp.sh" },
	{ ram_perc,		"RAM:%s%%",		NULL },
	{ ram_free,		"-%s ",		NULL },
	{ swap_perc,	"-S:%s%% | ",		NULL },
	{ disk_free,		"D:%s",		"/" },
	{ disk_free,		"-%s | ",		"/home" },
	{ run_command,		"V:%s | ",		"/home/ming/scripts/check_vol.sh" },
	{ run_command,		"L:%s | ",		"light" },
	//{ run_command,	"MIC:%s | ",		"/home/minghui/scripts/check_mic.sh" },
	{ battery_perc,		"B:%s%%",		"BAT1" },
	{ run_command,		"-%s/",		"/home/ming/scripts/battery_rate.sh" },
	{ battery_state,		"%s | ",		"BAT1" },
	{ datetime, 		"%s ",           	"%F %T" },

	/* function format          argument */
	// { cpu_perc,		"🌹:%s%%",		NULL },
	// { run_command,		"/%s ",		"/home/minghui/scripts/check_cpu_temp.sh" },
	// { ram_perc,		"💪:%s%%",		NULL },
	// { ram_free,		"/%s  ",		NULL },
	// { disk_free,		"💿:%s  ",		"/" },
	// { run_command,		"🔔:%s  ",		"/home/minghui/scripts/check_vol.sh" },
	// { run_command,		"🔥:%s  ",		"light" },
	// //{ run_command,	"MIC:%s | ",		"/home/minghui/scripts/check_mic.sh" },
	// { battery_perc,		":%s%%  ",		"BAT1" },

	// { run_command,		"Ker:%s | ",		"/home/minghui/scripts/check_ker.sh" },
	// { cpu_perc,		"CPU:%s%%",		NULL },
	// { run_command,		"/%s| ",		"/home/minghui/scripts/check_cpu_temp.sh" },
	// { ram_perc,		"RAM:%s%%",		NULL },
	// { ram_free,		"/%s% | ",		NULL },
	// { disk_free,		"D:%s | ",		"/" },
	// { run_command,		"V:%s | ",		"/home/minghui/scripts/check_vol.sh" },
	// { run_command,		"L:%s | ",		"light" },
	// //{ run_command,	"MIC:%s | ",		"/home/minghui/scripts/check_mic.sh" },
	// { battery_perc,		"B:%s%%",		"BAT1" },
	// // { datetime, 		"%s ",           	"%F %T" },
};
