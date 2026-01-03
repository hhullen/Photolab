#ifndef PICTUREPLANE_ITEM_H
#define PICTUREPLANE_ITEM_H

#include <QQuickRhiItem>
#include "gui/picture_render.h"

class PicturePlaneItem : public QQuickRhiItem {
    Q_OBJECT
    QML_NAMED_ELEMENT(PicturePlaneItem)
    Q_PROPERTY(float cutter READ getCutter WRITE setCutter NOTIFY cutterChanged)

public:
    QQuickRhiItemRenderer *createRenderer() {
        renderer_.reset(new PictureRender);
        return renderer_.get();
    }

    float getCutter() {
        return renderer_.get()->getCutter();
    }

    void setCutter(float value) {
        renderer_.get()->setCutter(value);
        emit cutterChanged(value);
        update();
    }

signals:
    void cutterChanged(float value);

private:
    std::unique_ptr<PictureRender> renderer_;
};

#endif // PICTUREPLANE_ITEM_H
