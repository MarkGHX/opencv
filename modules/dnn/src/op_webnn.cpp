// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2018-2019, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.

#include <fstream>
#include "op_webnn.hpp"

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/logger.hpp>

#include "opencv2/core/utils/filesystem.hpp"
#include "opencv2/core/utils/filesystem.private.hpp"

#include <opencv2/dnn/shape_utils.hpp>

namespace cv { namespace dnn {

#ifdef HAVE_WEBNN

template<typename T>
static inline std::vector<T> getShape(const Mat& mat)
{
    std::vector<T> result(mat.dims);
    for (int i = 0; i < mat.dims; i++)
        result[i] = (T)mat.size[i];
    return result;
}

// WebnnGraph

WebnnGraph::WebnnGraph()
{

}

bool WebnnGraph::isInitialized()
{

}

void WebnnGraph::init(Target targetId)
{

}

void WebnnGraph::forward(const std::vector<Ptr<BackendWrapper> >& outBlobsWrappers)
{

}

void WebnnGraph::createGraph(Target targetId)
{

}

// WebnnBackendNode

WebnnBackendNode::WebnnBackendNode(const std::vector<Ptr<BackendNode> >& nodes,
                                         Ptr<Layer>& cvLayer_, std::vector<Mat*>& inputs,
                                         std::vector<Mat>& outputs, std::vector<Mat>& internals)
    : BackendNode(DNN_BACKEND_WEBNN))
{
    // to do. Not necessary now.
}

WebnnBackendNode::WebnnBackendNode(std::shared_ptr<ml::Operand>&& _operand)
    : BackendNode(DNN_BACKEND_WEBNN), operand(std::move(_operand)) {}

WebnnBackendNode::WebnnBackendNode(std::shared_ptr<ml::Operand>&& _operand)
    : BackendNode(DNN_BACKEND_WEBNN), operand(_operand) {}

// WebnnBackendWrapper

WebnnBackendWrapper::WebnnBackendWrapper(const Mat& m)
{
    std::vector<uint32_t> shape = getShape<uint32_t>(m);
    if (m.type() == CV_16F)
    {
        std::unique_ptr<float16_t> dataBuffer = (float16_t*) m.data;
        buffer = dataBuffer;
        shapeStorer.type = ml::Operand::Float16;
        shapeStorer.dataShape = shape;
        descriptor = {shapeStorer.type, shapeStorer.dataShape, (uint32_t)shape.size()};
    }
    else if (m.type() == CV_32F)
    {
        std::unique_ptr<float32_t> dataBuffer = std::move((float32_t*) m.data);
        buffer = dataBuffer;
        shapeStorer.type = ml::Operand::Float32;
        shapeStorer.dataShape = shape;
        descriptor = {shapeStorer.type, shapeStorer.dataShape, (uint32_t)shape.size()};
    }
    else if (m.type() == CV_8U)
    {
        std::unique_ptr<uint8_t> dataBuffer = std::move((uint8_t*) m.data);
        buffer = dataBuffer;
        shapeStorer.type = ml::Operand::Uint8;
        shapeStorer.dataShape = shape;
        descriptor = {shapeStorer.type, shapeStorer.dataShape, (uint32_t)shape.size()};
    }
    else
        CV_Error(Error::StsNotImplemented, format("Unsupported data type %s", typeToString(m.type()).c_str()));
}

WebnnBackendWrapper::WebnnBackendWrapper(Ptr<BackendWrapper> wrapper)
{
    Ptr<WebnnBackendWrapper> webnnWrapper = wrapper.dynamicCast<WebnnBackendWrapper>();
    CV_Assert(!webnnWrapper.empty());
    buffer = webnnWrapper->buffer;
    descriptor = webnnWrapper->descriptor;
}

static Ptr<BackendWrapper> WebnnBackendWrapper::create(Ptr<BackendWrapper> wrapper)
{
    return Ptr<BackendWrapper>(new WebnnBackendWrapper(wrapper));
}

void WebnnBackendWrapper::copyToHost()
{
    CV_LOG_DEBUG(NULL, "WebnnBackendWrapper::copyToHost()");
}

void WebnnBackendWrapper::setHostDirty()
{
    CV_LOG_DEBUG(NULL, "WebnnBackendWrapper::setHostDirty()");
}

void * WebnnBackendWrapper::getBuffer()
{
    return buffer;
}

#else
void forwardWebnn(const std::vector<Ptr<BackendWrapper> >& outBlobsWrappers,
                   Ptr<BackendNode>& operand)
{
    CV_Assert(false && "WebNN is not enabled in this OpenCV build");
}

#endif

}
}