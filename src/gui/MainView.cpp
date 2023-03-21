#include "MainView.h"
#include "../themes/lv_theme.h"

MainView::MainView(lv_indev_t* indev, ButtonLabelBar* buttonLabelBar, BaseLvObject* baseLvObject) : ButtonLabelledLvObject(indev, NULL)
{
    this->buttonLabelBar = buttonLabelBar;
    this->buttonLabelledLvObject = buttonLabelledLvObject;
}

MainView::~MainView()
{
    this->destroyLvObj();
}

lv_obj_t* MainView::createLvObj(lv_obj_t* parent)
{    
    // get the style we'll need for the bar
    display_theme_styles_t* binary_styles = (display_theme_styles_t*)lv_disp_get_theme(lv_obj_get_disp(parent))->user_data;
    lv_style_t* no_scrollbar = &(binary_styles->no_scrollbar);
    lv_style_t* button_no_highlight_style = &(binary_styles->button_no_highlight);

    // add window to screen object and set it to be the exact size
    this->this_obj = lv_obj_create(parent);
    lv_obj_set_size(this->this_obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_obj_center(this->this_obj);
    lv_obj_update_layout(this->this_obj);

    // First create a menu bar, thought it maybe initially hidden
    lv_obj_t* menuBarObj = this->buttonLabelBar->createLvObj(this->this_obj);
    this->buttonLabelBar->setHidden();

    // Now lets do the screen objects for the gui
    this->buttonLabelledLvObject->createLvObj(this->this_obj);

    // Finally ensure the menubar is top
    lv_obj_move_foreground(menuBarObj);

    return this->this_obj;
}

void MainView::focusLvObj(BaseLvObject* defocusLvObj)
{
    this->buttonLabelBar->setAutoHide(false);
    this->buttonLabelBar->setShown();
    this->buttonLabelledLvObject->focusLvObj();
}

void MainView::destroyLvObj()
{
    this->buttonLabelledLvObject->destroyLvObj();
    BaseLvObject::destroyLvObj();    
}