/*********************************************************************
 *  
 *  OWNDRAW.H
 *
 *  This file contains the definitions and function prototypes for
 *  the owner draw list box functions.  These functions allow for
 *  list boxes to maintain column alignment while using proportional
 *  fonts.
 *                                          
 *  Copyright (c) 1992 by ASH Software, Inc.
 *                                                                    
 *  Update History                                                    
 *                                                                    
 *    07/03/92 - Created source code
 *                                                                    
 *********************************************************************/

#if !defined(OWNDRAW__H)

//  Define function constants

#define AVERAGE_WIDTH       0 // Retrieves lAveCharWidth from font
#define MAXIMUM_WIDTH       1 // Retrieves lMaxCharInc from font
#define MAX_TEXT_CHARS    255 // Maximum length of listbox string

//  Function prototypes

MRESULT EXPENTRY ODInitLBWidthHeight(HWND hListBox,SHORT sfCharWidth,
  SHORT sMaxNumberOfCharacters,LONG *lCharWidth);
MRESULT EXPENTRY ODDrawLBItems(POWNERITEM pOwnerItem,
  LONG lCharWidth,SHORT *psTabStops);

//  End of OWNDRAW.H

#define OWNDRAW__H
#endif


