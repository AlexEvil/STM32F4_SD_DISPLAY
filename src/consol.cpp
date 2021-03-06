///-----------------------------------------------------------------------------
#include <stdio.h>
///-----------------------------------------------------------------------------
#include "consol.h"
///-----------------------------------------------------------------------------
//UART_HandleTypeDef ConsolHandle;
///-----------------------------------------------------------------------------
static void cmd_cid(char *par);
static void cmd_help(char *par);
///-----------------------------------------------------------------------------
/// Command definitions structure
///-----------------------------------------------------------------------------
typedef struct scmd
{
    char val[8];
    void (*func)(char *par);
} SCMD;
///-----------------------------------------------------------------------------
/**
 *  @brief Consol help commands
 */
static const char help[] = 
{
    "+ command -------------------- function --------------------------------------+\n"
    "| CAP \"fname\" [/A]              | captures serial data to a file            |\n"
    "|                                 |  [/A option appends data to a file]       |\n"
    "| FILL \"fname\" [nnnn]           | create a file filled with text            |\n"
    "|                                 |  [nnnn - number of lines, default=1000]   |\n"
    "| TYPE \"fname\"                  | displays the content of a text file       |\n"
    "| REN \"fname1\" \"fname2\"       | renames a file 'fname1' to 'fname2'       |\n"
    "| COPY \"fin\" [\"fin2\"] \"fout\"| copies a file 'fin' to 'fout' file        |\n"
    "|                                 |  ['fin2' option merges 'fin' and 'fin2']  |\n"
    "| DEL \"fname\"                   | deletes a file                            |\n"
    "| DIR [\"mask\"]                  | displays a list of files in the directory |\n"
    "| FORMAT [label [/FAT32]]         | formats the device                        |\n"
    "|                                 | [/FAT32 option selects FAT32 file system] |\n"
    "| ATTRIB \"fname\" [arg]          | change file attributes                    |\n"
    "| CID                             | read CID register from the memory card    |\n"
    "| HELP  or  ?                     | displays this help                        |\n"
    "+---------------------------------+-------------------------------------------+\n"
};
///-----------------------------------------------------------------------------
/**
 *  @brief Consol type commands
 */
static const SCMD cmd[] =
{
//    "CAP",    cmd_capture,
//    "TYPE",   cmd_type,
//    "REN",    cmd_rename,
//    "COPY",   cmd_copy,
//    "DEL",    cmd_delete,
//    "DIR",    cmd_dir,
//    "FORMAT", cmd_format,
//    "FILL",   cmd_fill,
//    "ATTRIB", cmd_attrib,
    "HELP",   cmd_help,
    "CID",    cmd_cid,
    "?",      cmd_help
};
///-----------------------------------------------------------------------------
/**
 *  @brief Display Command Syntax help
 *  @param char*       - pointer to a line
 *  @param int32_t     - count char iat current line
 *  @return true/false - successful/failed
 */
static void cmd_help(char *par)
{
    printf(help);
}
///-----------------------------------------------------------------------------
/**
 *  @brief Read and output SD card CID register
 *  @param char* - pointer to a line
 */
static void cmd_cid(char *par)
{
    SD_CardInfo Cid;
    if(StorageGetInfo(&Cid) == true)
    {
        //printf("Manufacturer ID: %d (0x%.2X)\n",cid.MID,cid.MID);
        //printf("OEM/Application ID: %c%c\n",cid.OID >> 8,cid.OID & 0xFF);
        //printf("Product name: %c%c%c%c%c\n",cid.PNM[0],cid.PNM[1],cid.PNM[2],cid.PNM[3],cid.PNM[4]);
        //printf("Product revision: %d.%d\n",cid.PRV >> 4,cid.PRV & 0x0F);
        //printf("Product serial number: 0x%X\n",cid.PSN);
        //printf("Manufacturing date: %d/%.2d\n",cid.MDT & 0x0F,cid.MDT >> 4);
    }
    else
    {
        printf("CID register read failed.\n");
    }
}
///-----------------------------------------------------------------------------
/**
 *  @brief Display Command Syntax help
 *  @param char*       - pointer to a line
 *  @param int32_t     - count char iat current line
 *  @return true/false - successful/failed
 */
void ConsolInit(void)
{
    UART_IO_Init();
}
///-----------------------------------------------------------------------------
void ConsolTest(char* cmd,uint16_t length)
{
    UART_IO_Write((uint8_t *)cmd,length,1000);
}








///-----------------------------------------------------------------------------

#ifdef MY_USE_CONSOL
///-----------------------------------------------------------------------------
///#include <stdio.h>  /// Standard I/O .h-file
///#include <ctype.h>  /// Character functions
///#include <string.h> /// String and memory functions

///#include "cmsis_os.h" /// CMSIS RTOS definitions
///#include "rl_fs.h"    /// FileSystem definitions
///-----------------------------------------------------------------------------

///-----------------------------------------------------------------------------
/// Command function prototypes
//static void cmd_capture (char *par);
//static void cmd_type    (char *par);
//static void cmd_rename  (char *par);
//static void cmd_copy    (char *par);
//static void cmd_delete  (char *par);
//static void cmd_dir     (char *par);
//static void cmd_format  (char *par);

//static void cmd_fill    (char *par);
//static void cmd_attrib  (char *par);

///-----------------------------------------------------------------------------
/// Local constants
static const char intro[] =
{
    "\n\n\n\n\n\n\n\n"
    "+-----------------------------------------------------------------------------+\n"
    "|                    FileSystem File Manipulation                             |\n"
};
///-----------------------------------------------------------------------------

///-----------------------------------------------------------------------------
#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))
///-----------------------------------------------------------------------------
/// Local variables
static char in_line[160];
///-----------------------------------------------------------------------------
/// Local Function Prototypes
static void dot_format(uint64_t val, char *sp);
static char *get_entry(char *cp, char **pNext);
///-----------------------------------------------------------------------------
/// Extract drive specification from the input string
static char *get_drive(char *src,char *dst,uint32_t dst_sz)
{
    uint32_t i, n;
    i = 0;
    n = 0;
    while(!n && src && src[i] && (i < dst_sz))
    {
        dst[i] = src[i];
        if (dst[i] == ':')
        {
            n = i + 1;
        }
        i++;
    }
    if(n == dst_sz)
    {
        n = 0;
    }
    dst[n] = '\0';
    return (src + n);
}
///-----------------------------------------------------------------------------
/**
 *  @brief Process input string for long or short name entry
 */
static char *get_entry(char *cp,char **pNext)
{
    char *sp, lfn = 0, sep_ch = ' ';
    if(cp == NULL)
    { /// skip NULL pointers
        *pNext = cp;
        return (cp);
    }
    for( ; *cp == ' ' || *cp == '\"'; cp++)
    { /// skip blanks and starting
        if(*cp == '\"')
        {
            sep_ch = '\"';
            lfn = 1;
        }
        *cp = 0;
    }
    for(sp = cp;*sp != CR && *sp != LF && *sp != 0;sp++)
    {
        if(lfn && *sp == '\"')
        {
            break;
        }
        if(!lfn && *sp == ' ' )
        {
            break;
        }
    }
    for( ;*sp == sep_ch || *sp == CR || *sp == LF;sp++)
    {
        *sp = 0;
        if(lfn && *sp == sep_ch)
        {
            sp ++;
            break;
        }
    }
    /// next entry
    *pNext = (*sp) ? sp : NULL;
    return (cp);
}
///-----------------------------------------------------------------------------
/**
 *  @brief Print size in dotted fomat
 *  @param uint64_t - input value
 *  @param char*    - output buffer
 */
static void dot_format(uint64_t val,char *sp)
{
    if(val >= (uint64_t)1e12)
    {
        sp  += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e12));
        val %= (uint64_t)1e12;
        sp  += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e9));
        val %= (uint64_t)1e9;
        sp  += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e6));
        val %= (uint64_t)1e6;
        sprintf(sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
        return;
    }
    if(val >= (uint64_t)1e9)
    {
        sp  += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e9));
        val %= (uint64_t)1e9;
        sp  += sprintf (sp,"%03d.",(uint32_t)(val/(uint64_t)1e6));
        val %= (uint64_t)1e6;
        sprintf(sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
        return;
    }
    if(val >= (uint64_t)1e6)
    {
        sp  += sprintf (sp,"%d.",(uint32_t)(val/(uint64_t)1e6));
        val %= (uint64_t)1e6;
        sprintf(sp,"%03d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
        return;
    }
    if(val >= 1000)
    {
        sprintf(sp,"%d.%03d",(uint32_t)(val/1000),(uint32_t)(val%1000));
        return;
    }
    sprintf(sp,"%d",(uint32_t)(val));
    return;
}


///-----------------------------------------------------------------------------
/**
 *  @brief Line Editor
 *  @param char*       - pointer to a line
 *  @param int32_t     - count char at current line
 *  @return true/false - successful/failed
 */
bool getline(char* line,int32_t n)
{
    int32_t cnt = 0;
    char c;
    do
    {
        c = getchar();
        switch(c)
        {
            /// ignore Control S/Q
            case CNTLQ     :
            case CNTLS     : break;
            case BACKSPACE :
            case DEL       :
                             if(cnt == 0)
                             {
                                break;
                             }
                             /// decrement count
                             cnt--;
                             /// and line pointer
                             line--;
                             /// echo backspace
                             putchar(BACKSPACE);
                             putchar(' ');
                             putchar(BACKSPACE);
                             fflush(stdout);
                             break;
            case ESC       : /// ESC - stop editing line
                             *line = 0;
                             return false;
            case CR        : /// CR - done, stop editing line
                             *line = c;
                             /// increment line pointer and count
                             line++;
                             cnt++;
                             c = LF;
            default        : /// echo and store character
                             putchar(*line = c);
                             fflush (stdout);
                             /// increment line pointer and count
                             line++;
                             cnt++;
                             break;
        }
    } /// check limit and CR
    while(cnt < n - 2  &&  c != LF);
    /// mark end of string
    *line = 0;
    return true;
}
///-----------------------------------------------------------------------------
#endif

#ifdef MY_USE_CONSOL 
/// Capture serial data to file
static void cmd_capture (char *par)
{
  char *fname, *next;
  bool  append, retv;
  FILE *f;

  fname = get_entry(par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  append = false;
  if (next) {
    par = get_entry (next, &next);
    if ((strcmp (par, "/A") == 0) ||(strcmp (par, "/a") == 0)) {
      append = true;
    }
    else {
      printf ("\nCommand error.\n");
      return;
    }
  }
  printf ((append) ? "\nAppend data to file %s" :
                     "\nCapture data to file %s", fname);
  printf("\nPress ESC to stop.\n");
  f = fopen (fname,append ? "a" : "w"); /* open a file for writing            */
  if (f == NULL) {
    printf ("\nCan not open file!\n");  /* error when trying to open file     */
    return;
  }
  do {
    retv = getline (in_line, sizeof (in_line));
    fputs (in_line, f);
  } while (retv == true);
  fclose (f);                         /* close the output file                */
  printf ("\nFile closed.\n");
}
///-----------------------------------------------------------------------------
/// Create a file and fill it with some text
static void cmd_fill (char *par) {
  char *fname, *next;
  FILE *f;
  int i,cnt = 1000;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  if (next) {
    par = get_entry (next, &next);
    if (sscanf (par,"%d", &cnt) == 0) {
      printf ("\nCommand error.\n");
      return;
    }
  }

  f = fopen (fname, "w");               /* open a file for writing            */
  if (f == NULL) {
    printf ("\nCan not open file!\n");  /* error when trying to open file     */
    return;
  } 
  for (i = 0; i < cnt; i++)  {
    fprintf (f, "This is line # %d in file %s\n", i, fname);
    if (!(i & 0x3FF)) printf("."); fflush (stdout);
  }
  fclose (f);                           /* close the output file              */
  printf ("\nFile closed.\n");
}
///-----------------------------------------------------------------------------
/// Read file and dump it to serial window
static void cmd_type (char *par) {
  char *fname,*next;
  FILE *f;
  int ch;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  printf("\nRead data from file %s\n",fname);
  f = fopen (fname,"r");                /* open the file for reading          */
  if (f == NULL) {
    printf ("\nFile not found!\n");
    return;
  }
 
  while ((ch = fgetc (f)) != EOF) {     /* read the characters from the file  */
    putchar (ch);                       /* and write them on the screen       */
  }
  fclose (f);                           /* close the input file when done     */
  printf ("\nFile closed.\n");
}
///-----------------------------------------------------------------------------
/// Rename a File
static void cmd_rename (char *par) {
  char *fname,*fnew,*next,dir;

  fname = get_entry (par, &next);
  if (fname == NULL) {
    printf ("\nFilename missing.\n");
    return;
  }
  fnew = get_entry (next, &next);
  if (fnew == NULL) {
    printf ("\nNew Filename missing.\n");
    return;
  }
  if (strcmp (fname,fnew) == 0) {
    printf ("\nNew name is the same.\n");
    return;
  }

  dir = 0;
  if (*(fname + strlen(fname) - 1) == '\\') {
    dir = 1;
  }

  if (frename (fname, fnew) == fsOK) {
    if (dir) {
      printf ("\nDirectory %s renamed to %s\n",fname,fnew);
    }
    else {
      printf ("\nFile %s renamed to %s\n",fname,fnew);
    }
  }
  else {
    if (dir) {
      printf ("\nDirectory rename error.\n");
    }
    else {
      printf ("\nFile rename error.\n");
    }
  }
}
///-----------------------------------------------------------------------------
/**
 *  @brief Copy a File
 */
static void cmd_copy(char *par)
{
    char *fname,*fnew,*fmer,*next;
    FILE *fin,*fout;
    uint32_t cnt,total;
    char buf[512];
    bool merge;
    fname = get_entry(par,&next);
    if(fname == NULL)
    {
        printf("\nFilename missing.\n");
        return;
    }
    fmer = get_entry(next,&next);
    if(fmer == NULL)
    {
        printf("\nNew Filename missing.\n");
        return;
    }
    fnew = get_entry(next,&next);
    if(fnew != NULL)
    {
        merge = true;
    }
    else
    {
        merge = false;
        fnew = fmer;
    }
    if((strcmp(fname,fnew) == 0) || (merge && strcmp (fmer,fnew) == 0))
    {
        printf ("\nNew name is the same.\n");
        return;
    }
    /// open the file for reading
    fin = fopen(fname,"r");
    if(fin == NULL)
    {
        printf("\nFile %s not found!\n",fname);
        return;
    }
    if(merge == false)
    {
        printf("\nCopy file %s to %s\n",fname,fnew);
    }
    else
    {
        printf("\nCopy file %s, %s to %s\n",fname,fmer,fnew);
    }
    /// open the file for writing
    fout = fopen(fnew,"w");
    if(fout == NULL)
    {
        printf("\nFailed to open %s for writing!\n",fnew);
        fclose(fin);
        return;
    }
    total = 0;
    while((cnt = fread(&buf,1,512,fin)) != 0)
    {
        fwrite(&buf,1,cnt,fout);
        total += cnt;
    }
    /// close input file when done
    fclose(fin);
    if(merge == true)
    {
        /// open the file for reading
        fin = fopen(fmer,"r");
        if(fin == NULL)
        {
            printf("\nFile %s not found!\n",fmer);
        }
        else
        {
            while((cnt = fread(&buf,1,512,fin)) != 0)
            {
                fwrite(&buf,1,cnt,fout);
                total += cnt;
            }
            fclose(fin);
        }
    }
    fclose(fout);
    dot_format(total,&buf[0]);
    printf("\n%s bytes copied.\n",&buf[0]);
    return;
}
///-----------------------------------------------------------------------------
/**
 *  @brief Delete a File
 */
static void cmd_delete(char *par)
{
    char *fname, *next;
    fsStatus status;
    fname = get_entry(par,&next);
    if(fname == NULL)
    {
        printf("\nFilename missing.\n");
        return;
    }
    status = fdelete(fname,next);
    if(status == fsOK)
    {
        printf("\nDelete successful.\n");
    }
    else
        if(status == fsFileNotFound)
        {
            printf("\nFile %s not found.\n",fname);
        }
    else
    {
        printf("\nDelete failed.\n");
    }
}
///-----------------------------------------------------------------------------
/**
 *  @brief Print a Directory
 */
static void cmd_dir(char *par)
{
    uint64_t fsize;
    uint32_t files,dirs,i;
    char temp[32],*mask,*next,ch;
    fsFileInfo info;

  mask = get_entry (par, &next);
  if (mask == NULL) {
    mask = "*.*";
  } else if ((mask[1] == ':') && (mask[2] == 0)) {
    mask[2] = '*'; 
    mask[3] = '.'; 
    mask[4] = '*'; 
    mask[5] = 0; 
  } else if ((mask[2] == ':') && (mask[3] == 0)) {
    mask[3] = '*'; 
    mask[4] = '.'; 
    mask[5] = '*'; 
    mask[6] = 0; 
  }

  printf ("\nFile System Directory...");
  files = 0;
  dirs  = 0;
  fsize = 0;
  info.fileID  = 0;
  while (ffind (mask,&info) == fsOK) {
    if (info.attrib & FS_FAT_ATTR_DIRECTORY) {
      i = 0;
      while (strlen((const char *)info.name+i) > 41) {
        ch = info.name[i+41];
        info.name[i+41] = 0;
        printf ("\n%-41s", &info.name[i]);
        info.name[i+41] = ch;
        i += 41;
      }
      printf ("\n%-41s    <DIR>       ", &info.name[i]);
      printf ("  %02d.%02d.%04d  %02d:%02d",
               info.time.day, info.time.mon, info.time.year,
               info.time.hr, info.time.min);
      dirs++;
    }
    else {
      dot_format (info.size, &temp[0]);
      i = 0;
      while (strlen((const char *)info.name+i) > 41) {
        ch = info.name[i+41];
        info.name[i+41] = 0;
        printf ("\n%-41s", &info.name[i]);
        info.name[i+41] = ch;
        i += 41;
      }
      printf ("\n%-41s %14s ", &info.name[i], temp);
      printf ("  %02d.%02d.%04d  %02d:%02d",
               info.time.day, info.time.mon, info.time.year,
               info.time.hr, info.time.min);
      fsize += info.size;
      files++;
    }
  }
  if (info.fileID == 0) {
    printf ("\nNo files...");
  }
  else {
    dot_format (fsize, &temp[0]);
    printf ("\n              %9d File(s)    %21s bytes", files, temp);
  }
  dot_format (ffree(mask), &temp[0]);
  if (dirs) {
    printf ("\n              %9d Dir(s)     %21s bytes free.\n", dirs, temp);
  }
  else {
    printf ("\n%56s bytes free.\n",temp);
  }
}
///-----------------------------------------------------------------------------
/// Format Device
static void cmd_format (char *par) {
  char  label[12];
  char  drive[4];
  int   retv;

  par = get_drive (par, drive, 4);

  printf ("\nProceed with Format [Y/N]\n");
  retv = getchar();
  if (retv == 'y' || retv == 'Y') {
    /* Format the drive */
    if (fformat (drive, par) == fsOK) {
      printf ("Format completed.\n");
      if (fvol (drive, label, NULL) == 0) {
        if (label[0] != '\0') {
          printf ("Volume label is \"%s\"\n", label);
        }
      }
    }
    else {
      printf ("Formatting failed.\n");
    }
  }
  else {
    printf ("Formatting canceled.\n");
  }
}
///-----------------------------------------------------------------------------
/// Change file attributes
static void cmd_attrib (char *par) {
  char *arg, *path, *next, *sp;

  path = par;
  next = par;
  arg  = par;

  /* Parse input string for parameters */
  do {
    sp = get_entry (next, &next);

    if (sp == 0 && next == 0) {
      printf ("\nCommand error.\n");
      return;
    }

    if (next) {
      if (*next == '+' || *next == '-') {
        *(next - 1) = ' ';
      }
      else {
        path = next;
      }
    }
  }
  while (sp && next);

  if (fattrib (path, arg) != fsOK) {
    printf ("Failed to change file attributes.\n");
  }
  else {
    printf ("File attributes changed.\n");
  }
}
#endif
