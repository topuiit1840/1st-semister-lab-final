
#include <stdio.h>
#include <stdlib.h>

#include <iup.h>
#include <iupim.h>
#include <im.h>
#include <im_image.h>

#include "../include/custom.h"

int isAppStateValid()
{
  if (!state.undoImage && !state.currentImage && !state.currentImageFile)
  {
    Ihandle *msg_box = IupMessageDlg();

    IupSetAttribute(msg_box, "DIALOGTYPE", "WARNING");
    IupSetAttribute(msg_box, "TITLE", "Warning!!!");
    IupSetAttribute(msg_box, "BUTTONS", "OK");
    IupSetAttribute(msg_box, "VALUE", "You haven't selected an image yet!!!\nBye bye!!!\n");

    IupPopup(msg_box, IUP_CURRENT, IUP_CURRENT);
    IupDestroy(msg_box);
    
    return 0;
  }

  return 1;
}

void freeState()
{
  if (state.currentImageFile)
  {
    printf("%s\n", state.currentImageFile);
    free(state.currentImageFile);
  }

  if (state.currentImage)
  {
    imImageDestroy(state.currentImage);
  }
  
  if (state.undoImage)
  {
    imImageDestroy(state.undoImage);
  }
}

void printState()
{
  printf("Filename : %s\n", state.currentImageFile);
  printf("CurrentImage: %p\n", state.currentImage);
  printf("UndoImage: %p\n", state.undoImage);
  printf("ImageWidget: %p\n", state.imageWidget);
}