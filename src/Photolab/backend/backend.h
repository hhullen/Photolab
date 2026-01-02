#ifndef BACKEND_H
#define BACKEND_H

#include <QImage>

#include "controller/backend_interface.h"

class Backend: public IBackend {
public:
    Backend(const Backend&) = delete;
    Backend& operator=(const Backend&) = delete;
    Backend(Backend&&) = delete;
    Backend& operator=(Backend&&) = delete;

    static Backend &Get() {
        static Backend object;
        return object;
    }

    QImage &GetInitialImage() override;
    QImage &GetEditedImage() override;

private:
    Backend();
    QImage img1_;
    QImage img2_;
} ;

#endif // BACKEND_H
