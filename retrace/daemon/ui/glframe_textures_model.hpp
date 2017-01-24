/**************************************************************************
 *
 * Copyright 2016 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Sirisha Gandikota <sirisha.gandikota@intel.com>
 **************************************************************************/

#ifndef _GLFRAME_TEXTURES_MODEL_HPP_
#define _GLFRAME_TEXTURES_MODEL_HPP_

#include <QList>
#include <QObject>
#include <QQmlListProperty>
#include <QString>

#include <map>
#include <string>
#include <vector>

#include "glframe_retrace_interface.hpp"
#include "glframe_traits.hpp"

namespace glretrace {

class QSelection;
class QTexturesValue : public QObject,
                NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(uint32_t textureId READ textureId NOTIFY onTextureId)
  Q_PROPERTY(uint32_t textureUnit READ textureUnit NOTIFY onTextureUnit)
  Q_PROPERTY(uint32_t textureWidth READ textureWidth NOTIFY onTextureWidth)
  Q_PROPERTY(uint32_t textureHeight READ textureHeight NOTIFY onTextureHeight)
  Q_PROPERTY(uint32_t textureDataFormat READ textureDataFormat NOTIFY
onTextureDataFormat)
  Q_PROPERTY(uint32_t textureDataType READ textureDataType NOTIFY
onTextureDataType)
  Q_PROPERTY(uint32_t mipMap READ mipMap NOTIFY onMipMap)

 public:
  QTexturesValue();
  explicit QTexturesValue(QObject *p);
  void setTextureId(const uint32_t textureId);
  void setTextureUnit(const uint32_t textureUnit);
  void setTextureWidth(const uint32_t textureWidth);
  void setTextureHeight(const uint32_t textureHeight);
  void setTextureDataFomat(const uint32_t textureDataFormat);
  void setTextureDataType(const uint32_t textureDataType);
  void setMipMap(const uint32_t mipMap);

  uint32_t textureId() const { return m_textureId; }
  uint32_t textureUnit() const { return m_textureUnit; }
  uint32_t textureWidth() const { return m_textureWidth; }
  uint32_t textureHeight() const { return m_textureHeight; }
  uint32_t textureDataFormat() const { return m_textureDataFormat; }
  uint32_t textureDataType() const { return m_textureDataType; }
  uint32_t mipMap() const { return m_mipMap; }

 signals:
  void onTextureId();
  void onTextureUnit();
  void onTextureWidth();
  void onTextureHeight();
  void onTextureDataFormat();
  void onTextureDataType();
  void onMipMap();

 private:
  uint32_t m_textureId, m_textureUnit, m_textureWidth, m_textureHeight;
  uint32_t m_textureDataFormat, m_textureDataType, m_mipMap;
};

class QTexturesModel : public QObject, OnFrameRetrace,
                      NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(QQmlListProperty<glretrace::QTexturesValue> textures
             READ textures NOTIFY onTexturesChanged)
 public:
  QTexturesModel();
  ~QTexturesModel();
  void init(IFrameRetrace *r,
            QSelection *s,
            const std::vector<TexturesId> &ids,
            int render_count);
  void onFileOpening(bool needUpload,
                     bool finished,
                     uint32_t percent_complete) { assert(false); }
  void onShaderAssembly(RenderId renderId,
                        const ShaderAssembly &vertex,
                        const ShaderAssembly &fragment,
                        const ShaderAssembly &tess_control,
                        const ShaderAssembly &tess_eval,
                        const ShaderAssembly &geom,
                        const ShaderAssembly &comp) { assert(false); }
  void onRenderTarget(RenderId renderId, RenderTargetType type,
                      const uvec & pngImageData) { assert(false); }
  void onMetricList(const std::vector<MetricId> &ids,
                    const std::vector<std::string> &names,
                    const std::vector<std::string> &desc) { assert(false); }
  void onMetrics(const MetricSeries &metricData,
                 ExperimentId experimentCount,
                 SelectionId selectionCount) { }
  void onTexturesList(const std::vector<TexturesId> &ids) { assert(false); }
  void onTextures(RenderId renderId,
                     const std::vector<TextureData> &textures);
  void onShaderCompile(RenderId renderId,
                       ExperimentId experimentCount,
                       bool status,
                       const std::string &errorString) { assert(false); }
  void onApi(RenderId renderId,
             const std::vector<std::string> &api_calls) { assert(false); }
  void onError(const std::string &message) { assert(false); }

  QQmlListProperty<QTexturesValue> textures();
  void refresh();

 public slots:
  void onSelect(QList<int> selection);

 signals:
  void onTexturesChanged();

 private:
  IFrameRetrace *m_retrace;
  int m_render_count;
  SelectionId m_current_selection_count;
  RenderSelection m_render_selection;
  std::map<TexturesId, QTexturesValue*> m_textures;
  QList<QTexturesValue *> m_textures_list;
};

}  // namespace glretrace

#endif  // _GLFRAME_TEXTURES_MODEL_HPP_
