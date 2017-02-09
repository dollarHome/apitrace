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

#include "glframe_textures_model.hpp"

#include <sstream>
#include <string>
#include <vector>

#include "glframe_logger.hpp"
#include "glframe_os.hpp"
#include "glframe_retrace_model.hpp"

using glretrace::DEBUG;
using glretrace::IFrameRetrace;
using glretrace::FrameRetraceModel;
using glretrace::RenderId;
using glretrace::TextureData;
using glretrace::QTextures;
using glretrace::QRenderTextures;
using glretrace::QRenderTexturesModel;
using glretrace::ScopedLock;
using glretrace::SelectionId;
using glretrace::ShaderAssembly;

void
QTextures::setTextureId(const uint32_t textureId) {
  m_textureId = textureId;
}

void
QTextures::setTextureUnit(const uint32_t textureUnit) {
  m_textureUnit = textureUnit;
}

void
QTextures::setTextureWidth(const uint32_t textureWidth) {
  m_textureWidth = textureWidth;
}

void
QTextures::setTextureHeight(const uint32_t textureHeight) {
  m_textureHeight = textureHeight;
}

void
QTextures::setTextureDataFomat(const uint32_t textureDataFormat) {
  m_textureDataFormat = textureDataFormat;
}

void
QTextures::setTextureDataType(const uint32_t textureDataType) {
  m_textureDataType = textureDataType;
}

void
QTextures::setMipMap(const uint32_t mipMap) {
  m_mipMap = mipMap;
  emit mipMapChanged();
}


void
QRenderTextures::onTextures(const TextureData &textures) {
  /* int size = textures.size();
  for (int i = 0; i < size; i++) {
     QTextures textureObj;
     textureObj.onTextures(textures[i]);
     m_renderTextures.append(textureObj);
  } */
  printf("Inside QRenderTextures::onTextures()\n");
  m_renderTextures.onTextures(textures);
}

void
QRenderTexturesModel::onTextures(RenderId renderId,
                               SelectionId selectionCount,
                               const TextureData &texture) {
  ScopedLock s(m_protect);
  if (selectionCount != m_current_selection) {
    m_renders.clear();
    m_textureData.clear();
    m_render_strings.clear();
    m_current_selection = selectionCount;
  }
  assert(m_render_strings.size() == m_textureData.size());
  for (int i = 0; i < m_textureData.size(); ++i) {
    // if () { -- not sure what this check should be
      m_renders[i].push_back(renderId);
      m_render_strings[i].append(QString(",%1").arg(renderId.index()));
      emit onRendersChanged();
      return;
    //  }
  }

  // program not found
  m_renders.push_back(std::vector<RenderId>());
  m_renders.back().push_back(renderId);
  m_render_strings.push_back(QString("%1").arg(renderId.index()));
  m_textureData.push_back(std::vector<TextureData>());
  m_textureData.back().push_back(TextureData());
  m_textureData.back().back() = texture;

  printf("Inside QRenderTexturesModel::onTextures()\n");
  if (m_render_strings.size() == 1)
    setIndexDirect(0);

  emit onRendersChanged();
}

QStringList
QRenderTexturesModel::renders() {
  ScopedLock s(m_protect);
  return m_render_strings;
}

void
QRenderTexturesModel::setIndexDirect(int index) {
  printf("Inside QRenderTexturesModel::setIndexDirect(), index=%d\n", index);
  m_textures.onTextures(m_textureData[index][0]);
}

void
QRenderTexturesModel::setIndex(int index) {
  ScopedLock s(m_protect);
  setIndexDirect(index);
}

void
QRenderTexturesModel::setRetrace(IFrameRetrace *retracer,
                               FrameRetraceModel *model) {
  m_retracer = retracer;
  m_retraceModel = model;
}



/*
QTexturesModel::QTexturesModel()
    : m_retrace(NULL), m_current_selection_count(SelectionId(0)) {
}

void
QTexturesModel::init(IFrameRetrace *r,
                    QSelection *qs,
                    const std::vector<TexturesId> &ids,
                    int render_count) {
  m_retrace = r;
  m_render_count = render_count;
  for (int i = 0; i < ids.size(); ++i) {
    QTextures *q = new QTextures(this);
    m_textures_list.append(q);
    m_textures[ids[i]] = q;
  }

  RenderSelection s;
  // request frame and initial textures
  s.id = SelectionId(0);
  s.series.push_back(RenderSequence(RenderId(0), RenderId(render_count)));
  printf("Inside QTexturesModel::init()\n");
  m_retrace->retraceAllTextures(s, ExperimentId(0), this);
  connect(qs, &QSelection::onSelect,
          this, &QTexturesModel::onSelect);
  emit onTexturesChanged();
}

QQmlListProperty<QTextures>
QTexturesModel::textures() {
  return QQmlListProperty<QTextures>(this, m_textures_list);
}

void
QTexturesModel::onTextures(RenderId renderId,
                           SelectionId selectionCount,
                     const std::vector<TextureData> &textures) {
  // Set the texture values here for the UI to pull
}

void
QTexturesModel::onSelect(QList<int> selection) {
  m_render_selection.clear();
  if (selection.empty()) {
    return;
  }
  m_render_selection.id = ++m_current_selection_count;
  int last_render = -5;
  for (auto i : selection) {
    if (i != last_render + 1) {
      if (last_render >= 0) {
        m_render_selection.series.back().end = RenderId(last_render + 1);
      }
      m_render_selection.series.push_back(RenderSequence(RenderId(i),
                                                         RenderId(0)));
    }
    last_render = i;
  }
  printf("Inside QTexturesModel::onSelect()\n");
  m_render_selection.series.back().end = RenderId(last_render + 1);
  m_retrace->retraceAllTextures(m_render_selection, ExperimentId(0), this);
}

void
QTexturesModel::refresh() {
  // retrace the textures for the full frame
  RenderSelection s;
  s.id = SelectionId(0);
  s.series.push_back(RenderSequence(RenderId(0), RenderId(m_render_count)));
  m_retrace->retraceAllTextures(s, ExperimentId(0), this);
  printf("Inside QTexturesModel::refresh()\n");

  // retrace the textures for the current selection
  if (!m_render_selection.series.empty())
    m_retrace->retraceAllTextures(m_render_selection,
                                 ExperimentId(0), this);
}

QTexturesModel::~QTexturesModel() {
  for (auto m : m_textures_list)
    delete m;
  m_textures_list.clear();
  m_textures.clear();
}
*/
