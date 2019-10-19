#include <raylib.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>

#define JOY_DEV "/dev/input/js0"

typedef struct Joystick
{
    bool left;
    bool right;
    bool up;
    bool down;
    bool triangle;
    bool square;
    bool x;
    bool circle;
    bool home;
    bool select;
    bool start;
    bool leftTop;
    bool leftBottom;
    bool rightTop;
    bool rightBottom;
    bool leftThumb;
    bool rightThumb;
    float leftAxis;
    float rightAxis;
    Vector2 leftJoy;
    Vector2 rightJoy;
} Joystick;


int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 600;

    // init Joystick
    int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
    char *button=NULL, name_of_joystick[80];
    struct js_event js;

    // Check if can open joystick dev file
    if ( ( joy_fd = open(JOY_DEV, O_RDONLY)) == -1)
    {
        printf("Couldn't open joystick\n");
        return -1;
    }

    // Get some data on the joystick
    ioctl( joy_fd, JSIOCGAXES, &num_of_axis);
    ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons);
    ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick);

    // No idea what this does but its in the example
    axis = (int *) calloc( num_of_axis, sizeof(int));
    button = (char *) calloc( num_of_buttons, sizeof(char));

    // Print info on screen
    printf("[INFO] Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n",
        name_of_joystick,
        num_of_axis,
        num_of_buttons );
    
    // Set nonblocking
    fcntl( joy_fd, F_SETFL, O_NONBLOCK); 

    Joystick joystick = { 0 };

    // init raylib
    InitWindow(screenWidth, screenHeight, "Joystick testing");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        // Update
        
        /* update joystick*/
        
        // Read state
        read(joy_fd, &js, sizeof(struct js_event));

        // see what to do with the event
        switch (js.type & ~JS_EVENT_INIT)
        {
            case JS_EVENT_AXIS:
            {
                axis[js.number] = js.value;
            } break;

            case JS_EVENT_BUTTON:
            {
                switch (js.number)
                {
                    case 0:
                    { joystick.x = js.value; } break;
                    case 1:
                    { joystick.circle = js.value; } break;
                    case 2:
                    { joystick.triangle = js.value; } break;
                    case 3:
                    { joystick.square = js.value; } break;
                    case 4:
                    { joystick.leftTop = js.value; } break;
                    case 5:
                    { joystick.rightTop = js.value; } break;
                    case 6:
                    { joystick.leftBottom = js.value; } break;
                    case 7:
                    { joystick.rightBottom = js.value; } break;
                    case 8:
                    { joystick.select = js.value; } break;
                    case 9:
                    { joystick.start = js.value; } break;
                    case 10:
                    { joystick.home = js.value; } break;
                    case 11:
                    { joystick.leftThumb = js.value; } break;
                    case 12:
                    { joystick.rightThumb = js.value; } break;
                    case 13:
                    { joystick.up = js.value; } break;
                    case 14:
                    { joystick.down = js.value; } break;
                    case 15:
                    { joystick.left = js.value; } break;
                    case 16:
                    { joystick.right = js.value; } break;
                    default: break;
                }
                // button[js.number] = js.value;
            } break;
        }

        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            
            // make it display something based on inputs
            
            
            DrawFPS(GetScreenWidth() - 100,GetScreenHeight() - 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}