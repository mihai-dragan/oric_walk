# oric_walk
Moving animated sprites in HIRES for CC65

This is the second installment in the series of experiments with creating Oric HIRES graphics and animation using the CC65 compiler. For the first step look [here](https://github.com/mihai-dragan/oric_sprite).

I am continuing with the attempt to see how much can be achieved without doing any X, Y computations (transformations from the two dimensions of the screen space to the one dimension of memory space) and without moving sprites in between byte boundaries. So, how can we get smooth movement on the X axis  without doing these computations? We hardcode it in the sprite itself. We create two animated sprites for each transition from one position to the next (6 pixels apart on the X axis):

![sprites with transition hardcoded](images/walking1.png)

As you can see, the animation has four frames but we have to double the sprite for each of them. It means that Oric has less work to do, but more memory is used.

In the code the sprite looks like this:

    byte walk[8][8] = {         // 4 frames, 2 bytes wide (12pixels), 8 pixels high, sprite animation
    { 70,70,68,78,85,74,74,81 },
    { 64,64,64,64,64,64,64,64 },
    { 64,65,65,65,67,69,66,68 },
    { 64,96,96,64,112,64,112,80 },
    { 64,64,64,64,64,64,64,64 },
    { 88,88,80,120,112,88,120,80 },
    { 64,64,64,64,64,64,64,64 },
    { 88,88,80,120,112,88,84,84 }

Since we saved some computation time with the above trick, at the expense of memory, we are going to calculate the flipping of the sprite (when moving from right to left). This is done with a bit of code that removes the first two bits that don't contain pixel data, then splits the remaining six bits into two 3 bits and uses a map called `flipmap` to revert each of them:

    byte flipmap[8] = { 0,4,2,6,1,5,3,7 };
    // flip the six bits holding the pixels
    byte flip(byte b) {
        byte r=b&0b00111111;
        r=(flipmap[r&0b111] << 3) | flipmap[r>>3];
        return r|0b01000000;
    }

The code that moves the sprite to the left or to the right in response to key presses is not optimal. It mixes timing logic, drawing and position tracking, but these are normally separate concerns handled by the main loop. This will be addressed in the next part of this series.
