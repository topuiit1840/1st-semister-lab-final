#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iup.h>
#include <iupim.h>
#include <im.h>
#include <im_image.h>

#include "../include/custom.h"

int crop_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  Ihandle *in_x = (Ihandle *)IupGetAttributeHandle(self, "CROP_X");
  Ihandle *in_y = (Ihandle *)IupGetAttributeHandle(self, "CROP_Y");
  Ihandle *in_w = (Ihandle *)IupGetAttributeHandle(self, "CROP_W");
  Ihandle *in_h = (Ihandle *)IupGetAttributeHandle(self, "CROP_H");

  char *vX = IupGetAttribute(in_x, "VALUE");
  char *vY = IupGetAttribute(in_y, "VALUE");
  char *vW = IupGetAttribute(in_w, "VALUE");
  char *vH = IupGetAttribute(in_h, "VALUE");

  int pos_x = (vX && strlen(vX) > 0) ? atoi(vX) : 0;
  int pos_y = (vY && strlen(vY) > 0) ? atoi(vY) : 0;
  int c_width = (vW && strlen(vW) > 0) ? atoi(vW) : 0;
  int c_height = (vH && strlen(vH) > 0) ? atoi(vH) : 0;

  int img_w = state.currentImage->width;
  int img_h = state.currentImage->height;

  if (c_width <= 0 || c_height <= 0 || pos_x >= img_w || pos_y >= img_h || pos_x < 0 || pos_y < 0)
  {
    Ihandle *msg_box = IupMessageDlg();
    IupSetAttribute(msg_box, "DIALOGTYPE", "WARNING");
    IupSetAttribute(msg_box, "TITLE", "Error!!!");
    IupSetAttribute(msg_box, "BUTTONS", "OK");
    IupSetAttribute(msg_box, "VALUE", "Invalid Crop Inputs");
    IupPopup(msg_box, IUP_CURRENT, IUP_CURRENT);
    IupDestroy(msg_box);
    return IUP_DEFAULT;
  }

  if (pos_x + c_width > img_w)
    c_width = img_w - pos_x;
  if (pos_y + c_height > img_h)
    c_height = img_h - pos_y;

  if (state.undoImage)
    imImageDestroy(state.undoImage);

  state.undoImage = imImageDuplicate(state.currentImage);
  imImage *img_cropped = imImageCreate(c_width, c_height, state.currentImage->color_space, state.currentImage->data_type);

  unsigned char *s_red = state.currentImage->data[0];
  unsigned char *s_grn = state.currentImage->data[1];
  unsigned char *s_blu = state.currentImage->data[2];

  unsigned char *d_red = img_cropped->data[0];
  unsigned char *d_grn = img_cropped->data[1];
  unsigned char *d_blu = img_cropped->data[2];

  for (int row = 0; row < c_height; ++row)
  {
    for (int col = 0; col < c_width; ++col)
    {
      int orig_col = pos_x + col;
      int orig_row = pos_y + row;

      int idx_src = (orig_row * img_w) + orig_col;
      int idx_dst = (row * c_width) + col;

      d_red[idx_dst] = s_red[idx_src];
      d_grn[idx_dst] = s_grn[idx_src];
      d_blu[idx_dst] = s_blu[idx_src];
    }
  }

  imImageDestroy(state.currentImage);
  state.currentImage = img_cropped;
  updateUIImage(self);

  return IUP_DEFAULT;
}

int brightness_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  Ihandle *in_box = IupGetAttributeHandle(self, "MY_INPUT_TXT");
  char *val_str = IupGetAttribute(in_box, "VALUE");

  if (!val_str || strlen(val_str) == 0)
    return IUP_DEFAULT;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int shift = atoi(val_str);

  if (shift < -255 || shift > 255)
  {
    Ihandle *msg_box = IupMessageDlg();
    IupSetAttribute(msg_box, "DIALOGTYPE", "WARNING");
    IupSetAttribute(msg_box, "TITLE", "Error!!!");
    IupSetAttribute(msg_box, "BUTTONS", "OK");
    IupSetAttribute(msg_box, "VALUE", "Invalid Brightness Adjustment Inputs");
    IupPopup(msg_box, IUP_CURRENT, IUP_CURRENT);
    IupDestroy(msg_box);
    return IUP_DEFAULT;
  }

  int total_pixels = state.currentImage->width * state.currentImage->height;
  unsigned char *p_r = state.currentImage->data[0];
  unsigned char *p_g = state.currentImage->data[1];
  unsigned char *p_b = state.currentImage->data[2];

  for (int i = 0; i < total_pixels; ++i)
  {
    int new_r = p_r[i] + shift;
    int new_g = p_g[i] + shift;
    int new_b = p_b[i] + shift;

    new_r = (new_r > 255) ? 255 : (new_r < 0 ? 0 : new_r);
    new_g = (new_g > 255) ? 255 : (new_g < 0 ? 0 : new_g);
    new_b = (new_b > 255) ? 255 : (new_b < 0 ? 0 : new_b);

    p_r[i] = (unsigned char)new_r;
    p_g[i] = (unsigned char)new_g;
    p_b[i] = (unsigned char)new_b;
  }

  updateUIImage(self);
  return IUP_DEFAULT;
}

int grayScale_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int total = state.currentImage->width * state.currentImage->height;

  unsigned char *c_r = state.currentImage->data[0];
  unsigned char *c_g = state.currentImage->data[1];
  unsigned char *c_b = state.currentImage->data[2];

  for (int k = 0; k < total; k++)
  {
    unsigned char lum = 0.299 * c_r[k] + 0.587 * c_g[k] + 0.114 * c_b[k];
    c_r[k] = lum;
    c_g[k] = lum;
    c_b[k] = lum;
  }

  updateUIImage(self);
  return IUP_DEFAULT;
};

int Inversion_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int total_len = state.currentImage->width * state.currentImage->height;
  unsigned char *p_r = state.currentImage->data[0];
  unsigned char *p_g = state.currentImage->data[1];
  unsigned char *p_b = state.currentImage->data[2];

  for (int idx = 0; idx < total_len; ++idx)
  {
    p_r[idx] = 255 - p_r[idx];
    p_g[idx] = 255 - p_g[idx];
    p_b[idx] = 255 - p_b[idx];
  }

  updateUIImage(self);
  return IUP_DEFAULT;
};

int horizontalFlip_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int img_w = state.currentImage->width;
  int img_h = state.currentImage->height;

  unsigned char *ch_r = state.currentImage->data[0];
  unsigned char *ch_g = state.currentImage->data[1];
  unsigned char *ch_b = state.currentImage->data[2];

  for (int row = 0; row < img_h; ++row)
  {
    for (int col = 0; col < img_w / 2; ++col)
    {
      int pos1 = row * img_w + col;
      int pos2 = row * img_w + (img_w - 1 - col);

      unsigned char swap = ch_r[pos1];
      ch_r[pos1] = ch_r[pos2];
      ch_r[pos2] = swap;

      swap = ch_g[pos1];
      ch_g[pos1] = ch_g[pos2];
      ch_g[pos2] = swap;

      swap = ch_b[pos1];
      ch_b[pos1] = ch_b[pos2];
      ch_b[pos2] = swap;
    }
  }

  updateUIImage(self);
  return IUP_DEFAULT;
};

int verticalFlip_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int img_w = state.currentImage->width;
  int img_h = state.currentImage->height;

  unsigned char *c_r = state.currentImage->data[0];
  unsigned char *c_g = state.currentImage->data[1];
  unsigned char *c_b = state.currentImage->data[2];

  for (int r = 0; r < img_h / 2; ++r)
  {
    for (int c = 0; c < img_w; ++c)
    {
      int loc1 = r * img_w + c;
      int loc2 = (img_h - 1 - r) * img_w + c;

      unsigned char t = c_r[loc1];
      c_r[loc1] = c_r[loc2];
      c_r[loc2] = t;

      t = c_g[loc1];
      c_g[loc1] = c_g[loc2];
      c_g[loc2] = t;

      t = c_b[loc1];
      c_b[loc1] = c_b[loc2];
      c_b[loc2] = t;
    }
  }

  updateUIImage(self);
  return IUP_DEFAULT;
};

int rotate90_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int img_w = state.currentImage->width;
  int img_h = state.currentImage->height;

  unsigned char *old_r = state.currentImage->data[0];
  unsigned char *old_g = state.currentImage->data[1];
  unsigned char *old_b = state.currentImage->data[2];

  imImage *rotated = imImageCreate(img_h, img_w, state.currentImage->color_space, state.currentImage->data_type);
  unsigned char *nu_r = rotated->data[0];
  unsigned char *nu_g = rotated->data[1];
  unsigned char *nu_b = rotated->data[2];

  for (int y = 0; y < img_h; ++y)
  {
    for (int x = 0; x < img_w; ++x)
    {
      int p_src = y * img_w + x;
      int p_dst = (img_w - 1 - x) * img_h + y;

      nu_r[p_dst] = old_r[p_src];
      nu_g[p_dst] = old_g[p_src];
      nu_b[p_dst] = old_b[p_src];
    }
  }

  imImageDestroy(state.currentImage);
  state.currentImage = rotated;
  updateUIImage(self);

  return IUP_DEFAULT;
};

int blur_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;

  if (state.undoImage)
    imImageDestroy(state.undoImage);
  state.undoImage = imImageDuplicate(state.currentImage);

  int img_w = state.currentImage->width;
  int img_h = state.currentImage->height;

  unsigned char *p_r = state.currentImage->data[0];
  unsigned char *p_g = state.currentImage->data[1];
  unsigned char *p_b = state.currentImage->data[2];

  imImage *blurred = imImageCreate(img_w, img_h, state.currentImage->color_space, state.currentImage->data_type);
  unsigned char *b_r = blurred->data[0];
  unsigned char *b_g = blurred->data[1];
  unsigned char *b_b = blurred->data[2];

  int off_x[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
  int off_y[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

  for (int row = 0; row < img_h; ++row)
  {
    for (int col = 0; col < img_w; ++col)
    {
      unsigned int r_tot = 0, g_tot = 0, b_tot = 0, count = 0;

      for (int m = 0; m < 9; ++m)
      {
        int n_row = row + off_x[m];
        int n_col = col + off_y[m];

        if (n_row >= 0 && n_row < img_w && n_col >= 0 && n_col < img_h)
        {
          int p_idx = n_row * img_w + n_col;
          r_tot += p_r[p_idx];
          g_tot += p_g[p_idx];
          b_tot += p_b[p_idx];
          count++;
        }
      }

      int d_idx = row * img_w + col;

      b_r[d_idx] = (unsigned char)(r_tot / count);
      b_g[d_idx] = (unsigned char)(g_tot / count);
      b_b[d_idx] = (unsigned char)(b_tot / count);
    }
  }

  imImageDestroy(state.currentImage);
  state.currentImage = blurred;
  updateUIImage(self);

  return IUP_DEFAULT;
};

int undo_clb(Ihandle *self)
{
  if (!isAppStateValid())
    return IUP_CLOSE;
  if (!state.undoImage)
    return IUP_DEFAULT;

  imImage *reverted_img = imImageDuplicate(state.undoImage);

  imImageDestroy(state.currentImage);
  imImageDestroy(state.undoImage);

  state.undoImage = NULL;
  state.currentImage = reverted_img;

  updateUIImage(self);

  return IUP_DEFAULT;
};