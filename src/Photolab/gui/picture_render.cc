#include "picture_render.h"
#include "controller/controller.h"

PictureRender::PictureRender() {
    vertexShader_ = LoadShader(vertexShadetPath);
    fragmentShader_ = LoadShader(fragmentShadetPath);
}

float PictureRender::getCutter() {
    return cutter_;
}

void PictureRender::setCutter(float value) {
    cutter_ = value;
}

void PictureRender::initialize(QRhiCommandBuffer *cb) {
    qDebug() << "INIT";
    if (rhi_ != rhi()) {
        pipeline_.reset();
        rhi_ = rhi();
    }

    if (!pipeline_) {
        QRhiResourceUpdateBatch *resourceUpdates = rhi_->nextResourceUpdateBatch();

        vertexBuffer_.reset(rhi_->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(vertexDataXYUV)));
        vertexBuffer_->create();
        resourceUpdates->uploadStaticBuffer(vertexBuffer_.get(), vertexDataXYUV);

        uniformBuffer_.reset(rhi_->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, sizeof(UniformData)));
        uniformBuffer_->create();

        sampler_.reset(rhi_->newSampler(QRhiSampler::Linear, QRhiSampler::Linear, QRhiSampler::None,
                                        QRhiSampler::ClampToEdge, QRhiSampler::ClampToEdge));
        sampler_->create();

        if (imageChanged_) {
            UploadTexture(resourceUpdates, Controller::Get().GetInitialImage(), initialPicture_);
            UploadTexture(resourceUpdates, Controller::Get().GetEditedImage(), editedPicture_);
            pipeline_.reset();
            imageChanged_ = false;
        }

        shaderBindings_.reset(rhi_->newShaderResourceBindings());
        shaderBindings_->setBindings({
            QRhiShaderResourceBinding::sampledTexture(Bindings::InitTex, QRhiShaderResourceBinding::StageFlag::FragmentStage, initialPicture_.get(), sampler_.get()),
            QRhiShaderResourceBinding::sampledTexture(Bindings::EditedTex, QRhiShaderResourceBinding::StageFlag::FragmentStage, editedPicture_.get(), sampler_.get()),
            QRhiShaderResourceBinding::uniformBuffer(Bindings::UniformBuf, QRhiShaderResourceBinding::StageFlag::FragmentStage, uniformBuffer_.get()),
        });
        shaderBindings_->create();

        pipeline_.reset(rhi_->newGraphicsPipeline());
        pipeline_->setShaderStages({
            {QRhiShaderStage::Type::Vertex, vertexShader_},
            {QRhiShaderStage::Type::Fragment, fragmentShader_},
        });
        pipeline_->setTopology(QRhiGraphicsPipeline::TriangleStrip);

        QRhiVertexInputLayout inputLayout;
        inputLayout.setBindings({{ bytesPerVertex }});
        inputLayout.setAttributes({
            {0, VertexAttributeLocation::XY, QRhiVertexInputAttribute::Float2, offsetXY},
            {0, VertexAttributeLocation::UV, QRhiVertexInputAttribute::Float2, offsetUV},
        });

        pipeline_->setVertexInputLayout(inputLayout);
        pipeline_->setShaderResourceBindings(shaderBindings_.get());
        pipeline_->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
        pipeline_->create();

        cb->resourceUpdate(resourceUpdates);
    }
}

void PictureRender::render(QRhiCommandBuffer *cb) {
    qDebug() << "render";
    QRhiResourceUpdateBatch *resourceUpdates = rhi_->nextResourceUpdateBatch();
    UniformData data{};
    data.cutter = cutter_;
    resourceUpdates->updateDynamicBuffer(uniformBuffer_.get(), 0, sizeof(data), &data);

    const QColor clearColor = QColor::fromRgbF(0.0f, 0.0f, 0.0f, 0.0f);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, resourceUpdates);

    cb->setGraphicsPipeline(pipeline_.get());
    const QSize outputSize = renderTarget()->pixelSize();
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));
    cb->setShaderResources();
    const QRhiCommandBuffer::VertexInput vbufBinding(vertexBuffer_.get(), 0);
    cb->setVertexInput(0, 1, &vbufBinding);
    cb->draw(vertexCount);

    cb->endPass();
}

void PictureRender::synchronize(QQuickRhiItem *item) {
    qDebug() << "sync";
}

void PictureRender::UpdatePipeline(QRhi *rhi) {
    shaderBindings_.reset(rhi->newShaderResourceBindings());
    shaderBindings_->setBindings({
        QRhiShaderResourceBinding::sampledTexture(Bindings::InitTex, QRhiShaderResourceBinding::StageFlag::FragmentStage, initialPicture_.get(), sampler_.get()),
        QRhiShaderResourceBinding::sampledTexture(Bindings::EditedTex, QRhiShaderResourceBinding::StageFlag::FragmentStage, editedPicture_.get(), sampler_.get()),
        QRhiShaderResourceBinding::uniformBuffer(Bindings::UniformBuf, QRhiShaderResourceBinding::StageFlag::FragmentStage, uniformBuffer_.get()),
    });
    shaderBindings_->create();

    pipeline_.reset(rhi->newGraphicsPipeline());
    pipeline_->setTopology(QRhiGraphicsPipeline::Topology::TriangleStrip);
    pipeline_->setShaderResourceBindings(shaderBindings_.get());
    pipeline_->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
    pipeline_->setShaderStages({
        QRhiShaderStage{QRhiShaderStage::Type::Vertex, vertexShader_},
        QRhiShaderStage{QRhiShaderStage::Type::Fragment, fragmentShader_},
    });

    QRhiVertexInputLayout layout;
    layout.setBindings({ bytesPerVertex });
    layout.setAttributes({
        QRhiVertexInputAttribute{0, VertexAttributeLocation::XY, QRhiVertexInputAttribute::Float2, offsetXY},
        QRhiVertexInputAttribute{0, VertexAttributeLocation::UV, QRhiVertexInputAttribute::Float3, offsetUV},
    });
    pipeline_->setVertexInputLayout(layout);
    pipeline_->create();
}

void PictureRender::UploadTexture(QRhiResourceUpdateBatch *rub, QImage &img, std::unique_ptr<QRhiTexture> &tex) {
    tex.reset(rhi_->newTexture(QRhiTexture::RGBA8, img.size(), 1, {QRhiTexture::Flag::RenderTarget}));
    tex->create();
    rub->uploadTexture(tex.get(), img.convertToFormat(QImage::Format_RGBA8888));
}

QShader PictureRender::LoadShader(const QString &name)
{
    QFile f(name);
    if (f.open(QIODevice::ReadOnly)) {
        return QShader::fromSerialized(f.readAll());
    }

    // emit ErrorOccured("Failed to load shader: "+name);
    qDebug() << "Failed to load shader: "+name;
    return QShader();
}
