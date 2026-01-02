#include "backend.h"

#include <QDebug>

Backend::Backend() {
    img1_ = QImage("C:/Users/hhullen/Pictures/stella.jpg", "RGBA8");
    img2_ = QImage("C:/Users/hhullen/Pictures/stella_neg.jpg", "RGBA8");
    qDebug() << img1_.isNull() << " - " << img2_.isNull();
}

QImage &Backend::GetInitialImage() {
    return img1_;
}

QImage &Backend::GetEditedImage() {
    return img2_;
}
