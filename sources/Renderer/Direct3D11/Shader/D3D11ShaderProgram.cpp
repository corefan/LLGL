/*
 * D3D11ShaderProgram.cpp
 * 
 * This file is part of the "LLGL" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ShaderProgram.h"
#include "D3D11Shader.h"
#include "../D3D11Types.h"
#include "../../CheckedCast.h"
#include <LLGL/Log.h>
#include <LLGL/VertexFormat.h>
#include <algorithm>
#include <stdexcept>


namespace LLGL
{


D3D11ShaderProgram::D3D11ShaderProgram()
{
}

template <typename T>
void InsertBufferDesc(std::vector<T>& container, const std::vector<T>& entries)
{
    for (const auto& desc : entries)
    {
        auto it = std::find_if(
            container.begin(), container.end(), 
            [&desc](const T& entry)
            {
                return (entry.index == desc.index);
            }
        );
        if (it == container.end())
            container.push_back(desc);
    }
}

void D3D11ShaderProgram::AttachShader(Shader& shader)
{
    /* Store D3D11 shader */
    auto shaderD3D = LLGL_CAST(D3D11Shader*, &shader);

    switch (shader.GetType())
    {
        case ShaderType::Vertex:
            vs_ = shaderD3D;
            vertexAttributes_ = vs_->GetVertexAttributes();
            break;
        case ShaderType::Fragment:
            ps_ = shaderD3D;
            break;
        case ShaderType::TessControl:
            ds_ = shaderD3D;
            break;
        case ShaderType::TessEvaluation:
            hs_ = shaderD3D;
            break;
        case ShaderType::Geometry:
            gs_ = shaderD3D;
            break;
        case ShaderType::Compute:
            cs_ = shaderD3D;
            break;
    }

    /* Add constant- and storage buffer descriptors */
    InsertBufferDesc(constantBufferDescs_, shaderD3D->GetConstantBufferDescs());
    InsertBufferDesc(storageBufferDescs_, shaderD3D->GetStorageBufferDescs());
}

bool D3D11ShaderProgram::LinkShaders()
{
    enum ShaderTypeMask
    {
        MaskVS = (1 << 0),
        MaskPS = (1 << 1),
        MaskDS = (1 << 2),
        MaskHS = (1 << 3),
        MaskGS = (1 << 4),
        MaskCS = (1 << 5),
    };

    /* Validate shader composition */
    linkError_ = LinkError::NoError;

    int flags = 0;

    auto MarkShader = [&](D3D11Shader* shader, ShaderTypeMask mask)
    {
        if (shader)
        {
            if (shader->GetHardwareShader().vs == nullptr)
                linkError_ = LinkError::ByteCode;
            flags |= mask;
        }
    };

    MarkShader(vs_, MaskVS);
    MarkShader(ps_, MaskPS);
    MarkShader(ds_, MaskDS);
    MarkShader(hs_, MaskHS);
    MarkShader(gs_, MaskGS);
    MarkShader(cs_, MaskCS);

    switch (flags)
    {
        case (MaskVS | MaskPS):
        case (MaskVS | MaskPS | MaskGS):
        case (MaskVS | MaskPS | MaskDS | MaskHS):
        case (MaskVS | MaskPS | MaskDS | MaskHS | MaskGS):
        case (MaskCS):
            break;
        default:
            linkError_ = LinkError::Composition;
            break;
    }

    return (linkError_ == LinkError::NoError);
}

std::string D3D11ShaderProgram::QueryInfoLog()
{
    switch (linkError_)
    {
        case LinkError::Composition:
            return "invalid composition of attached shaders";
        case LinkError::ByteCode:
            return "invalid shader byte code";
        default:
            break;
    }
    return "";
}

std::vector<VertexAttribute> D3D11ShaderProgram::QueryVertexAttributes() const
{
    return vertexAttributes_;
}

std::vector<ConstantBufferDescriptor> D3D11ShaderProgram::QueryConstantBuffers() const
{
    return constantBufferDescs_;
}

std::vector<StorageBufferDescriptor> D3D11ShaderProgram::QueryStorageBuffers() const
{
    return storageBufferDescs_;
}

std::vector<UniformDescriptor> D3D11ShaderProgram::QueryUniforms() const
{
    return {}; // dummy
}

static DXGI_FORMAT GetInputElementFormat(const VertexAttribute& attrib)
{
    try
    {
        return D3D11Types::Map(attrib);
    }
    catch (const std::exception& e)
    {
        throw std::invalid_argument(std::string(e.what()) + " (for vertex attribute \"" + attrib.name + "\")");
    }
}

static D3D11_INPUT_CLASSIFICATION GetInputClassification(bool perInstance)
{
    return (perInstance ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA);
}

void D3D11ShaderProgram::BindVertexAttributes(const std::vector<VertexAttribute>& vertexAttribs)
{
    inputElements_.clear();
    inputElements_.reserve(vertexAttribs.size());

    for (const auto& attrib : vertexAttribs)
    {
        D3D11_INPUT_ELEMENT_DESC elementDesc;
        {
            elementDesc.SemanticName            = attrib.name.c_str();
            elementDesc.SemanticIndex           = attrib.semanticIndex;
            elementDesc.Format                  = GetInputElementFormat(attrib);
            elementDesc.InputSlot               = 0;
            elementDesc.AlignedByteOffset       = attrib.offset;
            elementDesc.InputSlotClass          = GetInputClassification(attrib.perInstance);
            elementDesc.InstanceDataStepRate    = 0;
        }
        inputElements_.push_back(elementDesc);
    }
}

void D3D11ShaderProgram::BindConstantBuffer(const std::string& name, unsigned int bindingIndex)
{
    //todo...
}

void D3D11ShaderProgram::BindStorageBuffer(const std::string& name, unsigned int bindingIndex)
{
    //todo...
}

ShaderUniform* D3D11ShaderProgram::LockShaderUniform()
{
    return nullptr; // dummy
}

void D3D11ShaderProgram::UnlockShaderUniform()
{
    // dummy
}


} // /namespace LLGL



// ================================================================================