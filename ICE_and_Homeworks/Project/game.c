#include "game.h"
extern bool readyShoot;
extern uint8_t touch_event;


int score = 5;
int numBullets = 10;

_GameCharacter ufo = {52, //w
											36, //h
											120,UFO_Y_MAX, //x, y
											space_shipBitmaps,
											LCD_COLOR_RED,
											LCD_COLOR_GREEN2,
											"character",
											(239-(52/2)),
											52/2,
											false};

_GameCharacter octopus ={53, //w
												41, //h
												120,OCTOPUS_Y_MAX, //x, y
												octopus_Bitmap,
												LCD_COLOR_BLACK,
												LCD_COLOR_BLUE2,
												"character",
												OCTOPUS_X_MAX,
												OCTOPUS_X_MIN,
												false};

											

		
											
_GameObj shieldArray[] =
{	
	// Rectangle 0
	{50, 50, 1, 			// width, height, border
	100, 50, 					//x,y
	LCD_COLOR_YELLOW,		// fill
	BG_COLOR,  //border	
	"object",
	239 - 50,					// max x
	1,								// min y (top of screen)
	true,
	false},
	
	//Rectangle 1
	{30, 20, 1, 				// width, height, border
	50, 120, 						//x,y
	LCD_COLOR_BLACK,		// fill
	BG_COLOR,  	//border	
	"object",
	239 - 30,
	1,									// min y
	false,							// moveRight
	false}
																				
};				

uint8_t numShields = sizeof(shieldArray)/ sizeof(shieldArray[0]);

_GameObj bullet = {10, 10, 1, 			// width, height, border
									0, 0, 					//x,y
									LCD_COLOR_RED,		// fill
									BG_COLOR,  //border	
									"object",
									239 - 10,
									1,							//min y, probably take out
									false}; 


											
void move_Left(uint16_t xPos, 
							uint16_t yPos, 
							uint32_t num_pixels, 
							uint16_t minX,
							const char type[],
							void* ptr)
{
	// loop variable
	uint16_t i;
	
	
	// check if going to hit left of screen
	if ((int)(xPos - num_pixels) < minX){
		// draw saucer left until reach left of screen
		// flip its direction
		for(i = xPos; i >= minX; i--){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos); 
			else if (!strcmp(type, "object"))  drawObject((_GameObj*) ptr, i, yPos);
		}
	}
	else {
		// must save because xPos is updated every iteration in the loop
		uint16_t final_x = xPos - num_pixels;
		// draw saucer left the correct # of pixels
		// start at xPos and go until you hit (xPos - num_pixels)
		for(i = xPos; i >= final_x; i--){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos); 
			else if (!strcmp(type, "object"))  drawObject((_GameObj*) ptr, i, yPos);
		}
	}
}


void move_Right(uint16_t xPos, 
								uint16_t yPos, 
								uint32_t num_pixels, 
								uint16_t maxX,
								const char type[],
								void* ptr)
{
	// loop variable
	uint16_t i;
	
	// check if going to hit right of screen
	if ((xPos + num_pixels) > maxX){
		// draw saucer right until reach right of screen
		for(i = xPos; i <= maxX; i++){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos);
			else if (!strcmp(type, "object"))  drawObject((_GameObj*) ptr, i, yPos);
		}
	}
	else {
		// must save because xPos is updated every iteration in the loop
		uint16_t final_x = xPos + num_pixels;
		// draw saucer right the correct # of pixels
		// start at xPos and go until you hit (xPos + num_pixels)
		for(i = xPos; i <= final_x; i++){
			// update x, keep y fixed
			//lcd_clear_screen(LCD_COLOR_GREEN);
			if (!strcmp(type,"character")) drawCharacter((_GameCharacter*)ptr, i, yPos); 
			else if (!strcmp(type, "object"))  drawObject((_GameObj*) ptr, i, yPos);
		}
	}
}





void shootBullet (uint16_t xPos, 
								  uint16_t yPos, 
								  _GameObj* obj)
{	
	// loop variable
	uint16_t i,j;
	obj->yPos = yPos;
	obj->xPos = xPos;
	
	// while bullet hasnt reached top of screen or hit a shield
	while ( (obj->yPos > 1) && !obj->hit)
	{
		obj->yPos--;
		
		drawObject(obj, obj->xPos, obj->yPos);
		
		// Check if bullet has hit any shields
		 for (i = 0; i < numShields; i++)
		{
			//printf("bullet x: %d\n shield x: %d\n", 
						//obj->xPos, 
						//shieldArray[i].xPos);
			
					 if (obj->yPos == (shieldArray[i].yPos + shieldArray[i].height) &&
						   obj->xPos < (shieldArray[i].xPos + shieldArray[i].width) &&
							 (obj->xPos + obj->width) >= shieldArray[i].xPos)
					 {
						 obj->hit = true;
					 }
		}
		 for (j = 0; j<10000; j++){}
	}
	// erase bullet
	lcd_draw_box(obj->xPos, //x start
								obj->width, // x len
								obj->yPos, //y s start
								obj->height, // y len
								BG_COLOR, //border
								BG_COLOR, //fill
								obj->border_weight
								);
	obj->hit = false;
}


// draw the character based on struct
void drawCharacter(_GameCharacter* character, uint16_t x, uint16_t y) 
{		
	// update x and y struct variables
	character->yPos = y;
	character->xPos = x;
	lcd_draw_image
    (
        x,         							// X Pos
        character->width,        // Image Horizontal Width
        y,         							// Y Pos
        character->height,       // Image Vertical Height
        character->bitmap,       // Image
        character->fColor,       // Foreground Color
        character->bColor        // Background Color
    );
}


void drawObject(_GameObj* obj, uint16_t x, uint16_t y)
{
	//printf("x: %d\n", x);
	obj->yPos = y;
	obj->xPos = x;
	
	lcd_draw_box(
			x, //x start
			obj->width, // x len
			y, //y s start
			obj->height, // y len
			obj->bColor, //border
			obj->fColor, //fill
			obj->border_weight
		);
}




void checkShooting()
{
			int i;
 
				// after moving check if you should shoot
			if (readyShoot && (touch_event > 0))
				 {
					 // have to wait again until ready to shoot
					 readyShoot = false;
					 numBullets--;
					 shootBullet(octopus.xPos -5, octopus.yPos - octopus.width/2 -3, &bullet);
					 
					 // check if bullet is at bottom of any of the shields
						
					 /*
						// bullet reached top of screen so erase it
					  lcd_draw_box(
						bullet.xPos, //x start
						bullet.width, // x len
						bullet.yPos, //y s start
						bullet.height, // y len
						BG_COLOR, //border
						BG_COLOR, //fill
						bullet.border_weight
						);
					 */
				 }
}