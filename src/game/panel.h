#pragma once
#include <cstdint>
#include <ctime>
#include <windows.h>

class DrawArea;
class Panel;
struct PanelVTable;

struct PanelNode {
public:
  Panel* panel;
  PanelNode* prev;
  PanelNode* next;
};

class Panel {
public:
  enum class PositionMode {
    Fixed = 0,
    Variable = 1,
    Centered = 2,
    Left = 3,
    Top = 4,
    Right = 5,
    Bottom = 6,
    LeftTop = 7,
    LeftBottom = 8,
    RightTop = 9,
    RightBottom = 10,
  };

  enum class RedrawMode {
    None = 0,
    Normal = 1,
    Full = 2,
  };

  enum class ZOrder {
    Position = 0,
    Top = 1,
    Bottom = 2,
  };

protected:
  PanelVTable* vtable_ = (PanelVTable*)0x642A90;
  Panel* prev_panel_ = nullptr;
  Panel* prev_modal_ = nullptr;
  int32_t x_offset_ = 0;
  int32_t y_offset_ = 0;
  int32_t width_ = 0;
  int32_t height_ = 0;
  char* panel_name_ = nullptr;
  DrawArea* draw_area_ = nullptr;
  RECT rect_;
  PositionMode position_mode_ = PositionMode::Fixed;
  RedrawMode redraw_mode_ = RedrawMode::Full;
  Panel* current_child_ = nullptr;
  Panel* parent_ = nullptr;
  Panel* left_panel_ = nullptr;
  Panel* top_panel_ = nullptr;
  Panel* right_panel_ = nullptr;
  Panel* bottom_panel_ = nullptr;
  PanelNode* panel_node_ = nullptr;
  PanelNode* first_child_ = nullptr;
  PanelNode* last_child_ = nullptr;
  Panel* tab_prev_panel_ = nullptr;
  Panel* tab_next_panel_ = nullptr;
  int32_t has_mouse_capture_ = 0;
  int32_t is_active_ = 1;
  int32_t is_visible_ = 0;
  int32_t tab_stop_ = 0;
  int32_t is_focused_ = 0;
  int32_t overlapping_children_ = 1;
  int32_t handle_mouse_input_ = 1;
  int32_t just_drawn_ = 0;
  int32_t enabled_ = 1;
  HRGN clip_region_;
  RECT render_rect_;
  int32_t left_border_ = 0;
  int32_t top_border_ = 0;
  int32_t right_border_ = 0;
  int32_t bottom_border_ = 0;
  int32_t min_width_ = 0;
  int32_t max_width_ = 0;
  int32_t min_height_ = 0;
  int32_t max_height_ = 0;
  int32_t mouse_hold_interval_ = 250;
  int32_t mouse_move_tolerance_ = 0;
  int32_t mouse_down_x_ = 0;
  int32_t mouse_down_y_ = 0;
  int32_t mouse_down_ctrl_ = 0;
  int32_t mouse_down_shift_ = 0;
  time_t mouse_down_time_ = 0;
  int32_t error_code_ = 0;
  ZOrder z_order_ = ZOrder::Position;
  int32_t display_changed_count_ = 0;
  int32_t help_string_id_ = -1;
  int32_t help_page_id_ = -1;
  int8_t mouse_action_ = 0;
  int8_t mouse_down_button_ = 0;
  int8_t panel_type_ = 0;
  uint8_t color_ = 0;
  bool fill_in_background_ = 0;
  bool clip_to_parent_ = 0;
  bool draw_rect2_flag_ = 0;
  bool need_restore_ = 0;

public:
  void setRedraw(RedrawMode mode = RedrawMode::Normal);
  void handleIdle();
  bool isInside(int32_t x, int32_t y);
  void drawRect(RECT& rect);
  void drawSetup(bool clear = false);
  void drawFinish();
};

struct PanelVTable {
  int32_t (__thiscall *destructor)(Panel*, char);
  bool (__thiscall *setup)(Panel*, DrawArea* draw_area, Panel* parent, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
  int32_t (__thiscall *setRect2)(Panel*, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
  HRGN (__thiscall *setRect)(Panel*, int32_t x, int32_t y, int32_t w, int32_t h);
  void (__thiscall *setColor)(Panel*, int8_t a2);
  void (__thiscall *setActive)(Panel*, int32_t active);
  int32_t (__thiscall *setPositioning)(Panel*, Panel::PositionMode mode, int32_t left, int32_t top, int32_t right, int32_t bottom, int32_t min_width, int32_t max_width, int32_t min_height, int32_t max_height, Panel* left_panel, Panel* top_panel, Panel* right_panel, Panel* bottom_panel);
  int32_t (__thiscall *setFixedPosition)(Panel*, int32_t a2, int32_t a3, int32_t a4, int32_t a5);
  void (__thiscall *setRedraw)(Panel*, Panel::RedrawMode redraw);
  void (__thiscall *setOverlappedRedraw)(Panel*, Panel* a2, Panel* a3, int32_t redraw_mode);
  int32_t (__thiscall *drawSetup)(Panel*, int32_t a2);
  int32_t (__thiscall *drawFinish)(Panel*);
  void (__thiscall *draw)(Panel*);
  int32_t (__thiscall *drawRect)(Panel*, RECT *rect);
  int32_t (__thiscall *drawOffset)(Panel*, int32_t a2, int32_t a3, RECT *rect);
  int32_t (__thiscall *drawRect2)(Panel*, RECT *rect);
  int32_t (__thiscall *drawOffset2)(Panel*, int32_t a2, int32_t a3, RECT *rect);
  void (__thiscall *paint)(Panel*);
  int32_t (__thiscall *wndProc)(Panel*, int32_t a2, bool a3, uint32_t a4, bool a5);
  int32_t (__thiscall *handleIdle)(Panel*);
  int32_t (__thiscall *handleSize)(Panel*, int32_t win_wid, int32_t win_hgt);
  int32_t (__thiscall *handlePaint)(Panel*);
  int32_t (__thiscall *handleKeyDown)(Panel*, int32_t key, int32_t count, int32_t alt_key, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *handleChar)(Panel*, uint32_t wParam, int32_t lParam);
  int32_t (__thiscall *handleCommand)(Panel*, uint32_t wParam, int32_t lParam);
  int32_t (__thiscall *handleUserCommand)(Panel*, uint32_t wParam, int32_t lParam);
  int32_t (__thiscall *handleTimerCommand)(Panel*, uint32_t wParam, int32_t lParam);
  int32_t (__thiscall *handleScroll)(Panel*, int32_t scroll_cmd, int32_t line_num);
  int32_t (__thiscall *handleMouseDown)(Panel*, int8_t mouse_button_in, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *handleMouseMove)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *handleMouseUp)(Panel*, int8_t mouse_button_in, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *handleMouseDblClick)(Panel*, int8_t mouse_button_in, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *handleMouseWheel)(Panel*, uint32_t msg, uint32_t wParam, int32_t lParam);
  int32_t (__thiscall *mouseWheelAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *handleMouseXbuttons)(Panel*, uint32_t msg, uint32_t wParam, int32_t lParam);
  int32_t (__thiscall *mouseXbuttonsAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseMoveAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseLeftDownAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseLeftHoldAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseLeftMoveAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseLeftUpAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseLeftDblClickAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseRightDownAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseRightHoldAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseRightMoveAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseRightUpAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *mouseRightDblClickAction)(Panel*, int32_t x, int32_t y, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *keyDownAction)(Panel*, int32_t key, int32_t count, int32_t alt_key, int32_t ctrl_key, int32_t shift_key);
  int32_t (__thiscall *charAction)(Panel*, int32_t key, int16_t count);
  int32_t (__thiscall *action)(Panel*, Panel* from_panel, int32_t action_in, uint32_t action_val1, uint32_t action_val2);
  void (__thiscall *getTrueRenderRect)(Panel*, RECT *drawRect);
  bool (__thiscall *isInside)(Panel*, int32_t x, int32_t y);
  void (__thiscall *setFocus)(Panel*, int32_t a2);
  void (__thiscall *setTabOrder2)(Panel*, Panel* a2, Panel* a3);
  void (__stdcall *setTabOrder)(Panel**list, int16_t order);
  void *renderArea;
  int8_t (__thiscall *getHelpInfo)(Panel*, char **help_out, uint32_t* size_out, int32_t a4, int32_t a5);
  void (__thiscall *stopSoundSystem)(Panel*);
  void (__thiscall *restartSoundSystem)(Panel*);
  void (__thiscall *takeSnapshot)(Panel*);
  void *handleReactivate;
  int32_t (__thiscall *pointingAt)(Panel*, int32_t x, int32_t y, int32_t* item, int32_t* sub_item, int32_t* extended, int32_t* string_id, char* text, int32_t max_text);
};
