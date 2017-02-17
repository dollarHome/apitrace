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
#include <mutex>
#include <string>
#include <vector>

#include "glframe_retrace_interface.hpp"
#include "glframe_traits.hpp"

namespace glretrace {

class QSelection;
class QTextures : public QObject,
                  NoCopy, NoAssign, NoMove {
  Q_OBJECT

 public:
  QTextures() {}
  ~QTextures() {}
  Q_PROPERTY(uint32_t textureId READ textureId)
  Q_PROPERTY(uint32_t textureUnit READ textureUnit)
  Q_PROPERTY(uint32_t textureWidth READ textureWidth)
  Q_PROPERTY(uint32_t textureHeight READ textureHeight)
  Q_PROPERTY(uint32_t textureDataFormat READ textureDataFormat)
  Q_PROPERTY(uint32_t textureDataType READ textureDataType)
  Q_PROPERTY(uint32_t mipMap READ mipMap NOTIFY mipMapChanged)
  Q_PROPERTY(std::vector<unsigned char> textureImage READ textureImage)

  void onTextures(const TextureData &texture) {
    m_textureId = texture.texture_id;
    m_textureUnit = texture.texture_unit;
    m_textureWidth = texture.texture_width;
    m_textureHeight = texture.texture_height;
    m_textureDataFormat = texture.texture_data_type;
    m_textureDataType = texture.texture_data_format;
    m_mipMap = texture.mipmap;
    printf("In QTextures::onTextures():");
    printf("texUnit = %d, width = %d, height = %d\n",
            m_textureUnit, m_textureWidth, m_textureHeight);
    if (texture.texture.size() > 0)
      m_textureImage.assign(texture.texture.front(), texture.texture.back());
  }

  // explicit QTextures(QObject *p);
  void setTextureId(const uint32_t textureId);
  void setTextureUnit(const uint32_t textureUnit);
  void setTextureWidth(const uint32_t textureWidth);
  void setTextureHeight(const uint32_t textureHeight);
  void setTextureDataFomat(const uint32_t textureDataFormat);
  void setTextureDataType(const uint32_t textureDataType);
  void setMipMap(const uint32_t mipMap);
  void setTextureImage(std::vector<unsigned char> textureImage);

  uint32_t textureId() const { return m_textureId; }
  uint32_t textureUnit() const { return m_textureUnit; }
  uint32_t textureWidth() const { return m_textureWidth; }
  uint32_t textureHeight() const { return m_textureHeight; }
  uint32_t textureDataFormat() const { return m_textureDataFormat; }
  uint32_t textureDataType() const { return m_textureDataType; }
  uint32_t mipMap() const { return m_mipMap; }
  std::vector<unsigned char>  textureImage() const { return m_textureImage; }

 signals:
  void mipMapChanged();

 private:
  uint32_t m_textureId, m_textureUnit, m_textureWidth, m_textureHeight;
  uint32_t m_textureDataFormat, m_textureDataType, m_mipMap;
  std::vector<unsigned char> m_textureImage;
};

//  Wrapper to enclose all the textures for a render
//  TO DO: This class may be redundant when using m_textures_list
//  Check and remove this code
class QRenderTextures : public QObject,
                        NoCopy, NoAssign, NoMove {
  Q_OBJECT

  Q_PROPERTY(QString renders READ renders NOTIFY onRenders)
 public:
  QRenderTextures() : m_currentSelectionId(0) {}
  ~QRenderTextures() {}

  void onTextures(const TextureData &texture);
  QString renders() { return m_renders; }
  QTextures *renderTextures() { return &m_renderTextures; }
 signals:
  void onRenders();
 private:
  QTextures m_renderTextures;
  SelectionId m_currentSelectionId;
  QString m_renders;
};

class FrameRetraceModel;
class QRenderTexturesModel : public QObject,
                             NoCopy, NoAssign, NoMove {
  Q_OBJECT
  Q_PROPERTY(glretrace::QRenderTextures* textures READ textures
             CONSTANT)
  Q_PROPERTY(QStringList renders
             READ renders NOTIFY onRendersChanged)
  Q_PROPERTY(QQmlListProperty<glretrace::QTextures> texturesList READ
             texturesList NOTIFY onTexturesChanged)

 public:
  QRenderTexturesModel() : m_retracer(NULL), m_retraceModel(NULL) {}
  ~QRenderTexturesModel();
  void setRetrace(IFrameRetrace *retracer,
                  FrameRetraceModel *model);
  QRenderTextures *textures() { return &m_textures; }
  void onTextures(RenderId renderId,
                  SelectionId selectionCount,
                  const TextureData &texture);
  QQmlListProperty<QTextures> texturesList();
  QStringList renders();
  Q_INVOKABLE void setIndex(int index);
 signals:
  void onRendersChanged();
  void onTexturesChanged();
 private:
  void setIndexDirect(int index);

  mutable std::mutex m_protect;
  QRenderTextures m_textures;
  QStringList m_render_strings;
  std::vector<std::vector<RenderId>> m_renders;
  std::vector< std::vector<TextureData>> m_textureData;
  QList<QTextures *> m_textures_list;
  SelectionId m_current_selection;
  IFrameRetrace *m_retracer;
  FrameRetraceModel *m_retraceModel;
};

}  // namespace glretrace

#endif  // _GLFRAME_TEXTURES_MODEL_HPP_
