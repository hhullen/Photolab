#ifndef PICTURERENDER_H
#define PICTURERENDER_H

#include <QQuickRhiItem>
#include <rhi/qrhi.h>
#include <QFile>

struct UniformData {
    float cutter;
    // float padding[3]; // Выравнивание до 16 байт (стандарт std140)
};

enum Bindings {
    InitTex = 0,
    EditedTex,
    UniformBuf,
};

enum VertexAttributeLocation {
    XY = 0,
    UV,
};

class PictureRender : public QQuickRhiItemRenderer {
public:
    PictureRender();

    float getCutter();
    void setCutter(float value);

private:
    float cutter_{0.5};
    bool imageChanged_{true};

    QRhi *rhi_{nullptr};
    QShader vertexShader_;
    QShader fragmentShader_;
    std::unique_ptr<QRhiSampler> sampler_;
    std::unique_ptr<QRhiBuffer> vertexBuffer_;
    std::unique_ptr<QRhiBuffer> uniformBuffer_;
    std::unique_ptr<QRhiTexture> editedPicture_;
    std::unique_ptr<QRhiTexture> initialPicture_;
    std::unique_ptr<QRhiGraphicsPipeline> pipeline_;
    std::unique_ptr<QRhiShaderResourceBindings> shaderBindings_;

    static constexpr int vertexCount{4};
    static constexpr int bytesPerVertex{vertexCount * sizeof(float)};
    static constexpr int offsetXY{0};
    static constexpr int offsetUV{2 * sizeof(float)};
    static constexpr float vertexDataXYUV[]{
        -1.0, -1.0, 0.0, 1.0,
        1.0, -1.0, 1.0, 1.0,
        -1.0,  1.0, 0.0, 0.0,
        1.0,  1.0, 1.0, 0.0
    };

    static constexpr char vertexShadetPath[]{":/shaders/PictureRender.vert.qsb"};
    static constexpr char fragmentShadetPath[]{":/shaders/PictureRender.frag.qsb"};

    void initialize(QRhiCommandBuffer *cb) override;
    void render(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;

    void UpdatePipeline(QRhi *rhi);
    void UploadTexture(QRhiResourceUpdateBatch *rub, QImage &img, std::unique_ptr<QRhiTexture> &tex);
    QShader LoadShader(const QString &name);

};

#endif // PICTURERENDER_H
