/*
 * main_flood.c
 * 
 * Copyright 2014 Michael Chen <chenm@chenm-Lenovo-IdeaPad-Y510P>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global Constants */
#define NORTH 8;
#define EAST  4;
#define SOUTH 2;
#define WEST  1;

#define STARTROW 0;
#define STARTCOL 0;
#define ENDROW 7;
#define ENDCOL 7;

/* Global Variables */
// Global array to store the cell values
unsigned char cellMap[255];
// Global array to store the wall bits
unsigned char wallMap[255];
// Global char to store current distance from end
unsigned char stepValue;
// Global array to act as stack
unsigned char cellStack[255];
// Global array to act as temporary storage
unsigned char tempCellStack[255];
// Global int to serve as a pointer to the top of the stack
// 0 means the stack is empty
unsigned int stackPointer, tempStackPointer;
// Global int to serve as the stack empty flag
unsigned int stackEmptyFlag, tempStackEmptyFlag;
// Global ints for current position
int currentRow, currentCol;
int startRow, startCol, endRow, endCol;

/* Function Prototypes */
void initializeMaze ();
void floodMaze ();
void setBoundaryWalls ();

int main(int argc, char **argv)
{
	// Initialize misc variables
	stepValue = 0;

	startRow = STARTROW;
	startCol = STARTCOL;
	endRow = ENDROW;
	endCol = ENDCOL;
	currentRow = STARTROW;
	currentCol = STARTCOL;
	
	// Initialize the maze
	initializeMaze ();
	// Set maze boundary walls
	setBoundaryWalls ();
	
	// Test Walls
	wallMap[16* endRow + endCol] |= NORTH;
	wallMap[16* endRow + endCol] |= WEST;
	wallMap[16* endRow + endCol] |= SOUTH;
	
	// Flood the maze
	floodMaze ();
	
	// Print the maze out
	for (int i = 0; i < 16; i++)
	{
		printf ("\n");
		for (int j = 0; j < 16; j++)
		{
			unsigned char current = cellMap[16* i + j];
			
			if (current < 10 && current < 100)
			{
				printf("  %d    ",cellMap[16* i + j]);
			}
			else if (current < 100)
			{
				printf("  %d   ",cellMap[16* i + j]);
			}
			else
			{
				printf("  %d  ",cellMap[16* i + j]);
			}
		}
		printf("\n");
	}
	
	return 0;
}

void initializeMaze ()
{
	for (int i = 0; i < 255; i++)
	{
		cellMap[i] = 255;
		wallMap[i] = 0;
	}
}

void setBoundaryWalls ()
{
	// NORTH
	for (int i = 0; i < 16; i++)
	{
		wallMap[i] |= NORTH;
	}
	// EAST
	for (int i = 15; i < 256; i += 16)
	{
		wallMap[i] |= EAST;
	}
	// SOUTH
	for (int i = 240; i < 256; i++)
	{
		wallMap[i] |= SOUTH;
	}
	// WEST
	for (int i = 0; i < 241; i += 16)
	{
		wallMap[i] |= WEST;
	}
}

void floodMaze ()
{
  // Set the bottom of each stack to garbage values
  cellStack[0] = 255;
  tempCellStack[0] = 255;

  // Initialize pointers to the top of each stack
  stackPointer = 1;
  tempStackPointer = 1;

  // Push the destination cell onto the stack
  cellStack[stackPointer] = (16 * endRow) + endCol;
  stackPointer++;
  // Signal that the stack is not empty
  stackEmptyFlag = 1;
  // Signal that the stack is indeed empty
  tempStackEmptyFlag = 0;

  while (stackEmptyFlag != 0)
  {
    // Stop flooding if our cell has a value
    if (cellMap[16 * currentRow + currentCol] != 255)
    {
      break;
    }

    // Pop the cell off the stack
    unsigned char floodCell = cellStack[stackPointer - 1];
    
    if (cellMap[floodCell] != 255)
    {
		if(stackPointer == 1)
		{
		}
		else
		{
			stackPointer--;
		}
	}
	else
	{
		// Set the current cell value to the step path value
		cellMap[floodCell] = stepValue;
		
		printf ("Flood Cell: %d\n", floodCell);

		// Add all unvisited, available uneighbors to a temporary stack stack
		
		// Get all wall bits
		unsigned char checkNorthWall = wallMap[floodCell] & (1 << 3);
		unsigned char checkEastWall = wallMap[floodCell] & (1 << 2);
		unsigned char checkSouthWall = wallMap[floodCell] & (1 << 1);
		unsigned char checkWestWall = wallMap[floodCell] & (1 << 0);
		
		// Check NORTH Cell
		if (checkNorthWall == 0 && cellMap[floodCell + 16] == 255)
		{
		  tempCellStack[tempStackPointer] = floodCell + 16;
		  // Update temp stack pointer
		  tempStackPointer++;
		  // Flag that it's no longer empty
		  tempStackEmptyFlag = 0;
		}
		// EAST Cell
		if (checkEastWall == 0 && cellMap[floodCell + 1] == 255)
		{
		  tempCellStack[tempStackPointer] = floodCell + 1;
		  // Update temp stack pointer
		  tempStackPointer++;
		  // Flag that it's no longer empty
		  tempStackEmptyFlag = 0;
		}
		// SOUTH Cell
		if (checkSouthWall == 0 && cellMap[floodCell - 16] == 255)
		{
		  tempCellStack[tempStackPointer] = floodCell - 16;
		  // Update temp stack pointer
		  tempStackPointer++;
		  // Flag that it's no longer empty
		  tempStackEmptyFlag = 0;
		}
		// WEST Cell
		if (checkWestWall == 0 && cellMap[floodCell - 1] == 255)
		{
		  tempCellStack[tempStackPointer] = floodCell - 1;
		  // Update temp stack pointer
		  tempStackPointer++;
		  // Flag that it's no longer empty
		  tempStackEmptyFlag = 0;
		}
	}
	

    // Check if this is the last pointer in the stack
    // If so, switch the empty flag on
    if (stackPointer == 1)
    {
      // Check if the secondary stack is empty
      if (tempStackEmptyFlag == 0)
      {
        // Replace primary stack with secondary stack
        for (int i = 0; i < tempStackPointer; i++)
        {
          cellStack[i] = tempCellStack[i];
        }

        stackPointer = tempStackPointer;

        // Reset secondary stack pointer and flag
        tempStackPointer = 1;
        tempStackEmptyFlag = 1;
        
        // Update the step value
        stepValue++;
      }
      // Otherwise, all stacks are empty
      else
      {
        stackEmptyFlag = 1;
      }

    }
    // If not, simply decrement the pointer value
    else
    {
      stackPointer--;
    }
    
    // Print stack for debug
    printf ("Current Stack\n");
    for (int i = 0; i < stackPointer; i++)
    {
		printf ("Stack Member: %d\n", cellStack[i]);
	}

  }
}

