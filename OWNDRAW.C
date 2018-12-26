/*********************************************************************
 *
 *  COMPANEL.C
 *
 *  These functions add additional functionality to the listbox
 *  class provided by OS/2.  These functions will cause text to be
 *  aligned in columns even when using a porportional spaced font.
 *
 *  Compiled with MS C6.0ax using compiler switches
 *
 *    /W3 /Alfu /Gt64 /G2s /c /FPi87
 *
 *  Copyright (c) 1992 by ASH Software, Inc.
 *
 *  Update History
 *
 *    07/03/92 - Created source code
 *
 *********************************************************************/

#define INCL_WIN
#define INCL_GPI

#include <os2.h>
#include <string.h>
#include <owndraw.h>

/*********************************************************************
 *  ODInitLBWidthHeight
 *
 *  This function should be called to process the WM_MEASUREITEM
 *  message.  The return value from this function should be used
 *  as the return value for the WM_MEASUREITEM message.
 *
 *  Arguments:
 *
 *    hListBox =
 *      Handle to the listbox window
 *
 *    sfCharWidth =
 *      Flag indicating which character spacing to use
 *      (AVERAGE_WIDTH or MAXIMUM_WIDTH)
 *
 *    sCharNax =
 *      Maximum number of characters in any text string for the
 *      particular listbox
 *
 *    *lCharWidth =
 *      Pointer to a long which will contain the character spacing
 *      on return from the function
 *
 *  Returns:
 *
 *    This function returns a MRESULT with the high word containing
 *    the maximum length of a line of text and the low word
 *    containing the character spacing.
 *********************************************************************/

MRESULT EXPENTRY ODInitLBWidthHeight(HWND hListBox,SHORT sfCharWidth,
  SHORT sCharMax,LONG *lCharWidth)
{
  SHORT
    sCharWidth;

  FONTMETRICS
    fm;

  HPS
    hps;

  hps=WinGetPS(hListBox);
  GpiQueryFontMetrics(hps,(LONG)sizeof(FONTMETRICS),&fm);
  WinReleasePS(hps);
  if (sfCharWidth == AVERAGE_WIDTH)
    sCharWidth=(SHORT)fm.lAveCharWidth;
  else
    sCharWidth=(SHORT)fm.lMaxCharInc;
  *lCharWidth=(LONG)sCharWidth;

  //  Return to the calling program

  return(
    MRFROM2SHORT(fm.lMaxBaselineExt,sCharMax*sCharWidth));
}

/*********************************************************************
 *  ODDrawLBItems
 *
 *  This function should be called to process the WM_DRAWITEM message.
 *  The return value from this function should be used as the return
 *  value for the WM_DRAWITEM message.  This function will align text
 *  in columns based on the column numbers provided in the tab stop
 *  array.  The function will expand embedded tabs by inserting spaces
 *  to reach the next tab stop.  Expanding tabs will slow the listbox
 *  operations, but may save significant amounts of memory.
 *
 *  Arguments:
 *
 *    pOwnerItem =
 *      Pointer to the OWNERITEM structure which is passed in
 *      the WM_DRAWITEM message as mp2
 *
 *    lCharWidth =
 *      The character spacing which is usually obtained by the
 *      call to ODInitLBWidthHeight
 *
 *    *psTabStops =
 *      Pointer to an array of tab stops.  The last tab stop
 *      must be 0 to indicate the end of the array.  The tab
 *      stops number the character positions from 1 to n.  There
 *      is no limit on the number of tab stops allowed.
 *
 *  Returns:
 *
 *    The function returns TRUE indicating that the function has
 *    drawn the text.  The function does not do the highlighting,
 *    but allows the system to perform the default highlighting.
 *
 *  Caution - Remember to make the tab stop arrary and the character
 *            width static or external so that the values will remain
 *            unchanged.
 *********************************************************************/

MRESULT EXPENTRY ODDrawLBItems(POWNERITEM pOwnerItem,
  LONG lCharWidth,SHORT *psTabStops)
{
  SHORT
    sTab,
    sLoop,
    sNumberOfChars,
    sTabCharPos,
    sNextCharPos,sPrevCharPos;

  LONG
    lSaveXLeft;

  CHAR
    cChar,
    cTempReadString[MAX_TEXT_CHARS],
    cTempString[MAX_TEXT_CHARS];

  if (pOwnerItem->fsState == pOwnerItem->fsStateOld)
    {

    //  Text needs to be drawn

    WinSendMsg(pOwnerItem->hwnd,LM_QUERYITEMTEXT,
      MPFROM2SHORT(pOwnerItem->idItem,MAX_TEXT_CHARS),
      MPFROMP(cTempReadString));
    lSaveXLeft=pOwnerItem->rclItem.xLeft;

    //  Prepare the output string by copying characters from the
    //  input string and by expanding any tabs found

    sNextCharPos=0;
    for (sLoop=0; sLoop<=(SHORT)strlen(cTempReadString); sLoop++)
      {
      cChar=cTempReadString[sLoop];
      if (cChar == '\t')
        {

        // Expand the tab to spaces

        sTab=-1;
        do
          {
          sTab++;
          sTabCharPos=psTabStops[sTab];
          } while ((sTabCharPos < sNextCharPos) && (sTabCharPos != 0));
        sTabCharPos--;
        if (sNextCharPos < sTabCharPos)
          for (; sNextCharPos<sTabCharPos; sNextCharPos++)
            cTempString[sNextCharPos]=' ';
        }
      else
        {

        // Copy the characters from the input string to the output
        // string

        cTempString[sNextCharPos]=cChar;
        sNextCharPos++;
        }
      }

    //  Align the text on the tab stops

    sTab=-1;
    sNextCharPos=1;
    do
      {
      sTab++;
      sPrevCharPos=sNextCharPos;
      sNextCharPos=psTabStops[sTab];
      if (sNextCharPos > 0)
        sNumberOfChars=sNextCharPos-sPrevCharPos;
      else
        sNumberOfChars=0xFFFF;
      pOwnerItem->rclItem.xLeft += (LONG)sPrevCharPos*lCharWidth;
      WinDrawText(pOwnerItem->hps,sNumberOfChars,
        &cTempString[sPrevCharPos-1],&pOwnerItem->rclItem,
        0,0,DT_LEFT|DT_VCENTER|DT_ERASERECT|DT_TEXTATTRS);
      }
    while (psTabStops[sTab] > 0);

    //  Let the system perform default highlighting

    pOwnerItem->rclItem.xLeft=lSaveXLeft;
    if (pOwnerItem->fsState)
      pOwnerItem->fsStateOld=FALSE;
    }

  //  Return to the calling program indicating that the text has 
  //  been drawn

  return((MRESULT) TRUE);
}
