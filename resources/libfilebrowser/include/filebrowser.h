#include <unistd.h>

#define MAXJOLIET 255
#define MAXDISPLAY 45

typedef struct
{
	char dir[MAXPATHLEN]; // directory path of browserList
	int numEntries; // # of entries in browserList
	int selIndex; // currently selected index of browserList
	int pageIndex; // starting index of browserList page display
} BROWSERINFO;

typedef struct
{
	unsigned int length; // file length
	char isdir; // 0 - file, 1 - directory
	char filename[MAXJOLIET + 1]; // full filename
	char displayname[MAXDISPLAY + 1]; // name for browser display
} BROWSERENTRY;

BROWSERINFO browser;
BROWSERENTRY * browserList;
char rootdir[10];

int UpdateDirName();
int FileSortCallback(const void *f1, const void *f2);
void ResetBrowser();
int BrowserChangeFolder();
int BrowseDevice();
