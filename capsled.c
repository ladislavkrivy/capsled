#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef TARGET_UBUNTU
#define INPUT_DEVICE "/dev/input/event3"
#elif defined TARGET_DEBIAN
#define INPUT_DEVICE "/dev/input/event0"
#elif defined TARGET_ARCH
#define INPUT_DEVICE "/dev/input/event4"
#else
#define INPUT_DEVICE "/dev/input/event0"
#endif

#define KEY_PRESSED 1
#define KEY_RELEASED 0
#define True 1
#define False 0
#define USER_NOBODY 65534
#define GROUP_NOBODY 65534

int capslock = 0;

int main(int argc, char *argv[]) {
  struct input_event ev[64];
  int fd, error = False;
  size_t size = sizeof(struct input_event);
  FILE *ledfd;
  bool ledblink = False; // cache feature from cmdline

  // Open keyboard device.
  if ((fd = open(INPUT_DEVICE, O_RDONLY)) == -1) {
    perror("Cannot open keyboard input device");
    error = True;
  }

  // Open led file.
  if ((ledfd = fopen("/proc/acpi/ibm/led", "w")) == NULL) {
    perror("Cannot open led control file");
    error = True;
  }

  // Drop privileges
  if (setgid(GROUP_NOBODY) == -1) {
    perror("Cannot drop group privileges");
    error = True;
  }

  if (setuid(USER_NOBODY) == -1) {
    perror("Cannot drop user privileges");
    error = True;
  }

  if (error) {
    fprintf(stderr, "Fatal error: exiting..\n");

    exit(1);
  }
  if (argc > 1) {
    if (strcmp(argv[1], "-b") || strcmp(argv[1], "--ledblink")) {
      ledblink = True;
    }
  }

  // Set daemon mode.
  daemon(0, 0);

  while (1) {
    if (read(fd, ev, size * 64) < size) {
      perror("Error reading keystrokes...exiting!");
      exit(1);
    }

    if (ev[0].value == KEY_CAPSLOCK && ev[1].value == KEY_PRESSED) {
      if (capslock) {
        capslock = False;
        // turn on the LED
        fprintf(ledfd, "0 on");
        fflush(ledfd);
      } else {
        capslock = True;
        // turn off / ledblink the LED
        if (ledblink) {
          fprintf(ledfd, "0 ledblink");
        } else
          fprintf(ledfd, "0 off");
        fflush(ledfd);
      }
    }
  }

  // This code should never be executed.
  return 0;
}
