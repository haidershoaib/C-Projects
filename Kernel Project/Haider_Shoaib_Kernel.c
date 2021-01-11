#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  in readFile, writeFile;
  char fileName[32], fileName2[32];
  ssize_t from, to, f1, f2;
  void *buf = (char*) valloc(150);

  const char msg[] = "Enter the name of the file you want to copy: \n";
  write(0, msg, sizeof(msg)-1);

  f1 = read(0, fileName, sizeof fileName -1);
  fileName[f1-1] = '\0';

  // Open the file to be read if the file exists
  readFile = open(fileName, O_RDONLY);
  if (readFile < 0)
  {
      const char msg1[] = "Cannot open file\n";
      write(0, msg1, sizeof(msg1)-1);
      return 0;
  }

  const char msg2[] = "Enter the filename you want to write to: \n";
  write(0, msg2, sizeof(msg2)-1);

 f2 = read(0, fileName2, sizeof fileName2 -1);
 fileName2[f2-1] = '\0';

  // Open the file to copy the contents to if the file exists
  writeFile = open(fileName2, O_WRONLY);
  if (writeFile < 0)
  {
      const char msg3[] = "Cannot open file\n";
      write(0, msg3, sizeof(msg3)-1);
      return 0;
  }

    // Read contents from the read file and copy to the write file
    while (1) {
        from = read(readFile, buf, 150);
        if (from <= 0) {
            break;
        }
        to = write(writeFile, buf, from);
        if (to <= 0){
            break;
        }
    }
    const char msg4[] = "Contents successfully copied\n";
    write(0, msg4, sizeof(msg4)-1);

  // Close the files
  close(readFile);
  close(writeFile);
  return 0;
}