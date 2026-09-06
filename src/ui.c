#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iup.h>
#include <im.h>
#include <im_image.h>
#include <iupim.h>

#include "../include/custom.h"

void updateUIImage(Ihandle *self)
{
  if (!state.currentImage)
  {
    printf("Error : Couldn't update UI Image because no current image found\n");
    return;
  }

  if (!state.imageWidget)
  {
    printf("Error : couldn't imageWidget not initialized\n");
    return;
  }

  Ihandle *prev_handle = (Ihandle *)IupGetAttributeHandle(state.imageWidget, "IMAGE");
  Ihandle *next_handle = IupImageFromImImage(state.currentImage);

  if (!next_handle)
  {
    printf("Couldn't get new_img in updateUIImage()\n");
    return;
  }

  IupSetAttribute(state.imageWidget, "TITLE", NULL);
  IupSetAttributeHandle(state.imageWidget, "IMAGE", next_handle);

  IupUpdate(state.imageWidget);
  IupMap(state.imageWidget);
  IupRefresh(state.imageWidget);

  if (prev_handle)
  {
    IupDestroy(prev_handle);
  }
}