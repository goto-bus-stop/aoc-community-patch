#include "panel.h"
#include "draw_area.h"
#include "../auto_hook.h"

void Panel::setRedraw(Panel::RedrawMode mode) {
  this->vtable_->setRedraw(this, mode);
}

void Panel::handleIdle() {
  auto original = getMethod<void, Panel*>(0x561D10);
  original(this);
}

bool Panel::isInside(int32_t x, int32_t y) {
  return this->is_active_ &&
    x >= this->rect_.left && x <= this->rect_.right &&
    y >= this->rect_.top && y <= this->rect_.bottom;
}

void Panel::drawRect(RECT& rect) {
  auto prev_rect = this->rect_;
  this->rect_ = rect;
  this->vtable_->draw(this);
  this->rect_ = prev_rect;
}

void Panel::drawSetup(bool clear) {
  if (auto dc = this->draw_area_->deviceContext()) {
    SelectClipRgn(dc, this->clip_region_);
  }

  this->draw_area_->setClipRect(&this->rect_);
  if (clear) {
    this->draw_area_->clear(&this->rect_, this->color_);
  }
}

void Panel::drawFinish() {
  this->redraw_mode_ = RedrawMode::None;
  if (auto dc = this->draw_area_->deviceContext()) {
    SelectClipRgn(dc, nullptr);
  }
  this->draw_area_->setClipRect(nullptr);
  this->need_restore_ = false;
}
