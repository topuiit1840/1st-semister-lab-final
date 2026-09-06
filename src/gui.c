
#include <stdio.h>
#include <stdlib.h>

#include <iup.h>

#include <im.h>
#include <im_image.h>
#include <iupim.h>

#include "../include/custom.h"

void setupGui()
{
    Ihandle *main_win, *layout_v;

    Ihandle *menu_file, *opt_open, *opt_exit, *opt_save_as;
    Ihandle *main_menu, *menu_sub1;

    Ihandle *btn_bw = IupButton("Grayscale", NULL);
    Ihandle *btn_inv = IupButton("Inversion", NULL);
    Ihandle *btn_h_flip = IupButton("Horizontal Flip", NULL);
    Ihandle *btn_v_flip = IupButton("Vertical Flip", NULL);
    Ihandle *btn_rot_90 = IupButton("Rotate 90deg", NULL);
    Ihandle *btn_blur_fx = IupButton("Blur", NULL);
    Ihandle *btn_revert = IupButton("Undo", NULL);

    IupSetCallback(btn_bw, "ACTION", (Icallback)grayScale_clb);
    IupSetCallback(btn_inv, "ACTION", (Icallback)Inversion_clb);
    IupSetCallback(btn_h_flip, "ACTION", (Icallback)horizontalFlip_clb);
    IupSetCallback(btn_v_flip, "ACTION", (Icallback)verticalFlip_clb);
    IupSetCallback(btn_rot_90, "ACTION", (Icallback)rotate90_clb);
    IupSetCallback(btn_blur_fx, "ACTION", (Icallback)blur_clb);
    IupSetCallback(btn_revert, "ACTION", (Icallback)undo_clb);

    Ihandle *btn_bar = IupHbox(
        btn_bw, btn_inv, btn_h_flip, btn_v_flip, btn_rot_90, btn_blur_fx, btn_revert, NULL);

    IupSetAttribute(btn_bar, "GAP", "4");

    Ihandle *in_crop_x = IupText(NULL);
    IupSetAttribute(in_crop_x, "MASK", IUP_MASK_UINT);
    IupSetAttribute(in_crop_x, "VISIBLECOLUMNS", "4");

    Ihandle *in_crop_y = IupText(NULL);
    IupSetAttribute(in_crop_y, "MASK", IUP_MASK_UINT);
    IupSetAttribute(in_crop_y, "VISIBLECOLUMNS", "4");

    Ihandle *in_crop_w = IupText(NULL);
    IupSetAttribute(in_crop_w, "MASK", IUP_MASK_UINT);
    IupSetAttribute(in_crop_w, "VISIBLECOLUMNS", "4");

    Ihandle *in_crop_h = IupText(NULL);
    IupSetAttribute(in_crop_h, "MASK", IUP_MASK_UINT);
    IupSetAttribute(in_crop_h, "VISIBLECOLUMNS", "4");

    Ihandle *btn_crop_exe = IupButton("Crop Image", NULL);
    IupSetCallback(btn_crop_exe, "ACTION", (Icallback)crop_clb);

    IupSetAttributeHandle(btn_crop_exe, "CROP_X", in_crop_x);
    IupSetAttributeHandle(btn_crop_exe, "CROP_Y", in_crop_y);
    IupSetAttributeHandle(btn_crop_exe, "CROP_W", in_crop_w);
    IupSetAttributeHandle(btn_crop_exe, "CROP_H", in_crop_h);

    Ihandle *layout_crop = IupHbox(
        IupLabel("Crop - X: "), in_crop_x,
        IupLabel(" Y: "), in_crop_y,
        IupLabel(" Width: "), in_crop_w,
        IupLabel(" Height: "), in_crop_h,
        btn_crop_exe,
        NULL);

    IupSetAttribute(layout_crop, "ALIGNMENT", "ACENTER");
    IupSetAttribute(layout_crop, "MARGIN", "10x10");

    Ihandle *lbl_bright, *in_bright, *btn_bright, *layout_bright;
    lbl_bright = IupLabel("Brightness Adjustment (-255 to 255) : ");
    in_bright = IupText(NULL);
    btn_bright = IupButton("Apply", NULL);

    IupSetAttribute(in_bright, "MASK", IUP_MASK_INT);
    IupSetAttribute(in_bright, "VISIBLECOLUMNS", "5");

    IupSetAttributeHandle(btn_bright, "MY_INPUT_TXT", in_bright);
    IupSetCallback(btn_bright, "ACTION", (Icallback)brightness_clb);

    layout_bright = IupHbox(lbl_bright, in_bright, btn_bright, NULL);
    IupSetAttribute(layout_bright, "ALIGNMENT", "ACENTER");

    Ihandle *lbl_img = IupLabel("Image : ");

    state.imageWidget = IupLabel(NULL);
    IupSetAttribute(state.imageWidget, "IMAGE", "DUMMY_INIT_NAME");
    IupSetAttribute(state.imageWidget, "TITLE", NULL);
    IupSetAttribute(state.imageWidget, "EXPAND", "YES");
    IupSetAttribute(state.imageWidget, "ALIGNMENT", "ACENTER:ACENTER");

    opt_open = IupItem("Open", NULL);
    IupSetCallback(opt_open, "ACTION", (Icallback)open_clb);

    opt_save_as = IupItem("Save as", NULL);
    IupSetCallback(opt_save_as, "ACTION", (Icallback)saveAs_clb);

    opt_exit = IupItem("Exit", NULL);
    IupSetCallback(opt_exit, "ACTION", (Icallback)ext_clb);

    menu_file = IupMenu(opt_open, opt_save_as, IupSeparator(), opt_exit, NULL);
    menu_sub1 = IupSubmenu("File", menu_file);
    main_menu = IupMenu(menu_sub1, NULL);

    layout_v = IupVbox(btn_bar, layout_bright, layout_crop, lbl_img, state.imageWidget, NULL);
    IupSetAttribute(layout_v, "ALIGNMENT", "ACENTER");
    IupSetAttribute(layout_v, "GAP", "10");

    main_win = IupDialog(layout_v);
    IupSetAttributeHandle(main_win, "MENU", main_menu);
    IupSetAttribute(main_win, "TITLE", "Image Manipulation Software");
    IupSetAttribute(main_win, "EXPAND", "YES");
    IupSetAttribute(main_win, "SIZE", "500x300");

    IupShowXY(main_win, IUP_CENTER, IUP_CENTER);
    IupMainLoop();
}