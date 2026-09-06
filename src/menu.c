#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iup.h>
#include <iupim.h>
#include <im.h>
#include <im_image.h>

#include "../include/custom.h"

int open_clb(Ihandle *self)
{
  Ihandle *dialog_f = IupFileDlg();
  IupSetAttribute(dialog_f, "DIALOGTYPE", "OPEN");
  IupSetAttribute(dialog_f, "TITLE", "Select a BMP Image");
  IupSetAttribute(dialog_f, "EXTFILTER", "BMP Images (*.bmp)|*.bmp|");

  IupPopup(dialog_f, IUP_CENTER, IUP_CENTER);

  if (IupGetInt(dialog_f, "STATUS") != -1)
  {
    char *path_str = IupGetAttribute(dialog_f, "VALUE");
    size_t len = strlen(path_str);

    state.currentImageFile = (char *)malloc(sizeof(char) * len);
    strcpy(state.currentImageFile, path_str);

    int status_err = 0;
    state.currentImage = imFileImageLoadBitmap(state.currentImageFile, 0, &status_err);
    state.undoImage = imImageDuplicate(state.currentImage);

    if (status_err != 0)
    {
      printf("Error code in open_clb : %d\n", status_err);
    }

    updateUIImage(self);
  }

  IupDestroy(dialog_f);
  return IUP_DEFAULT;
}

int saveAs_clb(Ihandle *self)
{
  if (!isAppStateValid())
  {
    return IUP_DEFAULT;
  }

  Ihandle *dialog_f = IupFileDlg();
  IupSetAttribute(dialog_f, "DIALOGTYPE", "SAVE");
  IupSetAttribute(dialog_f, "TITLE", "Save image as...");
  IupSetAttribute(dialog_f, "EXTFILTER", "BMP Images (*.bmp)|*.bmp|");
  IupSetAttribute(dialog_f, "EXTDEFAULT", "bmp");

  IupPopup(dialog_f, IUP_CENTER, IUP_CENTER);

  if (IupGetInt(dialog_f, "STATUS") != -1)
  {
    char *path_str = IupGetAttribute(dialog_f, "VALUE");
    int status_err = imFileImageSave(path_str, "BMP", state.currentImage);
    Ihandle *msg_box = IupMessageDlg();

    if (status_err != 0)
    {
      IupSetAttribute(msg_box, "DIALOGTYPE", "WARNING");
      IupSetAttribute(msg_box, "TITLE", "Error!!!");
      IupSetAttribute(msg_box, "BUTTONS", "OK");
      IupSetAttribute(msg_box, "VALUE", "Couldn't save the image");
    }
    else
    {
      IupSetAttribute(msg_box, "DIALOGTYPE", "WARNING");
      IupSetAttribute(msg_box, "TITLE", "Success!!!");
      IupSetAttribute(msg_box, "BUTTONS", "OK");
      IupSetAttribute(msg_box, "VALUE", "Success fully saved the image");
    }

    IupPopup(msg_box, IUP_CURRENT, IUP_CURRENT);
    IupDestroy(msg_box);
    updateUIImage(self);
  }

  IupDestroy(dialog_f);
  return IUP_DEFAULT;
}

int ext_clb(Ihandle *self)
{
  return IUP_CLOSE;
}