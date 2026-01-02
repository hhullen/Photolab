#include "controller.h"

Controller::Controller()  {}

void Controller::setBackend(IBackend *back) {
    back_ = back;
}

QImage &Controller::GetInitialImage() {
    return back_->GetInitialImage();
}

QImage &Controller::GetEditedImage() {
    return back_->GetEditedImage();
}
