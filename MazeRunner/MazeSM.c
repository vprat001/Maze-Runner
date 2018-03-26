/*
 *
 * Created by Venkat Prathipati    861209938
 *
 */

enum maze_SM {wait, right, left, up, down} maze_state;
void mazeSM() {
	original = 0b11111111;
	originalup = 0b11111110;
	originaldown = 0b01111111;
	switch(maze_state) {//transitions
		case wait:
			if(joystick == 0x01)//right
			{
				if ((~patternRed[j][redcursorPos] & patternGreen[j][redcursorPos+1]) != 0x00)
				{
					maze_state = right;
				}
				else 
				{
					maze_state = wait;
				}	
			}
			else if (joystick == 0x02)//left
			{
				if ((~patternRed[j][redcursorPos] & patternGreen[j][redcursorPos-1]) != 0x00)
				{
					maze_state = left;
				}
				else 
				{
					maze_state = wait;
				}				
			}
			else if (joystick == 0x04)//up
			{
				if (((~((patternRed[j][redcursorPos]<<1)|0x01)) & (patternGreen[j][redcursorPos])) != 0x00)
				{
					maze_state = up;
				}
				else 
				{
					maze_state = wait;
				}			
			}
			else if (joystick == 0x08)//down
			{
				if (((~((patternRed[j][redcursorPos]>>1)|0x80)) & (patternGreen[j][redcursorPos])) != 0x00)
				{
					maze_state = down;
				}
				else 
				{
					maze_state = wait;
				}
			}
			break;
		case right:
			maze_state = wait;
			break;
		case left:
			maze_state = wait;
			break;
		case up:
			maze_state = wait;
			break;
		case down:
			maze_state = wait;
			break;
		default:
			maze_state = wait;
			break;
	}
	switch(maze_state){//actions
		case wait:
			break;
		case right:
			if(redcursorPos < 7)
			{
				patternRed[j][redcursorPos + 1] = patternRed[j][redcursorPos];
				patternRed[j][redcursorPos] = original;
				redcursorPos++;
			}
			else if(redcursorPos >= 7)
			{
				redcursorPos = 0;
				patternRed[j][0] = patternRed[j][7];
				patternRed[j][7] = original;
			}break;
		case left:
			if(redcursorPos > 0)
			{
				patternRed[j][redcursorPos - 1] = patternRed[j][redcursorPos];
				patternRed[j][redcursorPos] = original;
				redcursorPos--;
			}
			else if (redcursorPos <= 0)
			{
				redcursorPos = 7;
				patternRed[j][7] = patternRed[j][0];
				patternRed[j][0] = original;
			}break;
		case up:
			if(redcursorLoc < 7)
			{	
				patternRed[j][redcursorPos] = (~(patternRed[j][redcursorPos])) << 1;
				patternRed[j][redcursorPos] = ~(patternRed[j][redcursorPos]);
				redcursorLoc++;
			}
			else if (redcursorLoc >= 7)
			{
				patternRed[j][redcursorPos] =  originalup;
				redcursorLoc = 0;
			}break;
		case down:
			if(redcursorLoc > 0)
			{
				patternRed[j][redcursorPos] = (~(patternRed[j][redcursorPos])) >> 1;
				patternRed[j][redcursorPos] = ~(patternRed[j][redcursorPos]);
				redcursorLoc--;
			}
			else if (redcursorLoc <= 0)
			{
				patternRed[j][redcursorPos] =  originaldown;
				redcursorLoc = 7;
			}break;
		default:
			redcursorPos = 0;
			redcursorLoc = 0;
			break;
	}
}