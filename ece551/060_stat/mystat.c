#include <ctype.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/* This function changes the time to a string. */
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t = localtime(when);
  strftime(temp1, 512, "%Y-%m-%d %H:%M:%S", t);
  strftime(temp2, 32, "%z", t);
  snprintf(ans, 128, "%s.%09ld %s", temp1, ns, temp2);
  return ans;
}

/* This function returns the type of the file, and fill in the first char
 * of the permission string.
 * It takes 2 parameters:
 *   f (a struct stat of the input file got with lstat)
 *   permissStr0 (points at the buffer of permission string) */
char * getFileType(struct stat f, char * permissStr0) {
  char * fileType;
  switch (f.st_mode & S_IFMT) {
    case S_IFBLK:
      fileType = "block special file";
      *permissStr0 = 'b';
      break;
    case S_IFCHR:
      fileType = "character special file";
      *permissStr0 = 'c';
      break;
    case S_IFDIR:
      fileType = "directory";
      *permissStr0 = 'd';
      break;
    case S_IFIFO:
      fileType = "fifo";
      *permissStr0 = 'p';
      break;
    case S_IFLNK:
      fileType = "symbolic link";
      *permissStr0 = 'l';
      break;
    case S_IFREG:
      fileType = "regular file";
      *permissStr0 = '-';
      break;
    case S_IFSOCK:
      fileType = "socket";
      *permissStr0 = 's';
      break;
  }
  return fileType;
}

/* This function returns one char in a certain location of permission string.
 * It takes 3 parameters:
 *   f (a struct stat of the input file got with lstat)
 *   sign (sign of the permission status)
 *   chr (potential char for this permission status) */
char getPermissChr(struct stat f, int sign, char chr) {
  if ((f.st_mode & sign) != 0) {
    return chr;
  }
  else {
    return '-';
  }
}

/* Use this function to get the 2nd to 10th chars in the permission string.
 * It takes 2 parameters:
 *   f (a struct stat of the input file got with lstat)
 *   permissStr (points at the buffer of permission string) */
void getPermissStr(struct stat f, char * permissStr) {
  permissStr[1] = getPermissChr(f, S_IRUSR, 'r');
  permissStr[2] = getPermissChr(f, S_IWUSR, 'w');
  permissStr[3] = getPermissChr(f, S_IXUSR, 'x');
  permissStr[4] = getPermissChr(f, S_IRGRP, 'r');
  permissStr[5] = getPermissChr(f, S_IWGRP, 'w');
  permissStr[6] = getPermissChr(f, S_IXGRP, 'x');
  permissStr[7] = getPermissChr(f, S_IROTH, 'r');
  permissStr[8] = getPermissChr(f, S_IWOTH, 'w');
  permissStr[9] = getPermissChr(f, S_IXOTH, 'x');
  permissStr[10] = '\0';
}

/* Use this function to get the link string of a symbolic link file type.
 * It takes 1 parameter:
 *   filename (name of the input file) */
char * getLink(const char * filename) {
  char * linktarget = malloc(256 * sizeof(*linktarget));
  ssize_t len = readlink(filename, linktarget, 256);
  linktarget[len] = '\0';
  return linktarget;
}

/* This function prints the 1st line of the results.
 * It takes 2 patameters:
 *   f (struct stat got from f with lstat)
 *   filename (name of the input file) */
void printLine1(const char * filename, struct stat f) {
  printf("  File: %s", filename);
  if (S_ISLNK(f.st_mode)) {  // if the file is a symbolic link
    char * linktarget = getLink(filename);
    printf(" -> %s", linktarget);
    free(linktarget);
  }
  printf("\n");
}

/* This function prints the 3rd line of the results.
 * It takes 1 patameter:
 *   f (struct stat got from f with lstat) */
void printLine3(struct stat f) {
  if (S_ISCHR(f.st_mode) || S_ISBLK(f.st_mode)) {  // if the file is a device
    printf("Device: %lxh/%lud\tInode: %-10lu  Links: %-5lu Device type: %d,%d\n",
           f.st_dev,
           f.st_dev,
           f.st_ino,
           f.st_nlink,
           major(f.st_rdev),
           minor(f.st_rdev));
  }
  else {
    printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",
           f.st_dev,
           f.st_dev,
           f.st_ino,
           f.st_nlink);
  }
}

/* This function prints the stat results of one file.
 * It takes 1 patameter:
 *   filename (name of the input file) */
void printStat(const char * filename) {
  struct stat f;
  if (lstat(filename, &f) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }

  char * permissStr = malloc(11 * sizeof(*permissStr));
  char * fileType = getFileType(f, permissStr);  // get file type
  getPermissStr(f, permissStr);                  // get permission string

  char * aTime = time2str(&f.st_atime, f.st_atim.tv_nsec);
  char * mTime = time2str(&f.st_mtime, f.st_mtim.tv_nsec);
  char * cTime = time2str(&f.st_ctime, f.st_ctim.tv_nsec);

  // print the stat results
  printLine1(filename, f);  // Line 1
  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
         f.st_size,
         f.st_blocks,
         f.st_blksize,
         fileType);  // Line 2
  printLine3(f);     // Line 3
  printf("Access: (%04o/%s)  Uid: (%5d/%8s)   Gid: (%5d/%8s)\n",
         (f.st_mode & ~S_IFMT),
         permissStr,
         f.st_uid,
         getpwuid(f.st_uid)->pw_name,
         f.st_gid,
         getgrgid(f.st_gid)->gr_name);  // Line 4
  printf("Access: %s\n", aTime);        // Line 5
  printf("Modify: %s\n", mTime);        // Line 6
  printf("Change: %s\n", cTime);        // Line 7
  printf(" Birth: -\n");                // Line 8

  free(permissStr);
  free(aTime);
  free(mTime);
  free(cTime);
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    fprintf(stderr, "./mystat: missing operand\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++) {
    printStat(argv[i]);
  }

  return EXIT_SUCCESS;
}
