# cpond
![image](https://github.com/user-attachments/assets/a8dddebd-af3d-4e20-a346-9742b1322300)
creates procedurally animated fish to swim around your terminal

[youtube](https://www.youtube.com/watch?v=OaGTJ6GPloo)

## make

_note I used gcc_

1. git clone
2. ensure you have ncurses installed. (ncurses6 was used)
3. move into your local clone of the repo
4. run `make` to get the cpond executable

## usage
- to run `./cpond`

- `-b or --braille` allows you to toggle the use of braille characters
- `-c or --count` allows you to set the number of fish
- `--fps` allows you to change the fps by changing the delay between frames.
    - ex: `--fps 60` will set the delay between frames to 1s / 60.

while runnning

- type q to quit
- type p to toggle pause
