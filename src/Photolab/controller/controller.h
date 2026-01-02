#ifndef CONTROLLER_H
#define CONTROLLER_H


#include "backend_interface.h"

class Controller {
public:
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    static Controller &Get() {
        static Controller ctrl;
        return ctrl;
    }

    void setBackend(IBackend *back);

    QImage &GetInitialImage();
    QImage &GetEditedImage();

private:
    Controller();

    IBackend *back_;
};

#endif // CONTROLLER_H
