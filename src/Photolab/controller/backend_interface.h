#ifndef BACKEND_INTERFACE_H
#define BACKEND_INTERFACE_H

#include <QImage>

class IBackend {
public:
    IBackend() = default;
    virtual ~IBackend() = 0;
    IBackend(const IBackend &) = delete;
    IBackend &operator=(const IBackend &) = delete;

    virtual QImage &GetInitialImage() = 0;
    virtual QImage &GetEditedImage() = 0;
};

inline IBackend::~IBackend() {}

#endif // BACKEND_INTERFACE_H
