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

#include <string>
#include <vector>

#include "glframe_qselection.hpp"

using glretrace::ExperimentId;
using glretrace::IFrameRetrace;
using glretrace::TexturesId;
using glretrace::TextureData;
using glretrace::QTexturesValue;
using glretrace::QTexturesModel;
using glretrace::QSelection;
using glretrace::SelectionId;

QTexturesValue::QTexturesValue() : m_textureId(0), m_textureUnit(0), \
                                   m_textureWidth(0), m_textureHeight(0), \
                                   m_textureDataFormat(0), \
                                   m_textureDataType(0), m_mipMap(0) {
  assert(false);
}

QTexturesValue::QTexturesValue(QObject *p) {
  moveToThread(p->thread());
}

void
QTexturesValue::setTextureId(const uint32_t textureId) {
  m_textureId = textureId;
  emit onTextureId();
}

void
QTexturesValue::setTextureUnit(const uint32_t textureUnit) {
  m_textureUnit = textureUnit;
  emit onTextureUnit();
}

void
QTexturesValue::setTextureWidth(const uint32_t textureWidth) {
  m_textureWidth = textureWidth;
  emit onTextureWidth();
}

void
QTexturesValue::setTextureHeight(const uint32_t textureHeight) {
  m_textureHeight = textureHeight;
  emit onTextureHeight();
}

void
QTexturesValue::setTextureDataFomat(const uint32_t textureDataFormat) {
  m_textureDataFormat = textureDataFormat;
  emit onTextureDataFormat();
}

void
QTexturesValue::setTextureDataType(const uint32_t textureDataType) {
  m_textureDataType = textureDataType;
  emit onTextureDataType();
}

void
QTexturesValue::setMipMap(const uint32_t mipMap) {
  m_mipMap = mipMap;
  emit onMipMap();
}

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
    QTexturesValue *q = new QTexturesValue(this);
    m_textures_list.append(q);
    m_textures[ids[i]] = q;
  }

  RenderSelection s;
  // request frame and initial textures
  s.id = SelectionId(0);
  s.series.push_back(RenderSequence(RenderId(0), RenderId(render_count)));
  m_retrace->retraceAllTextures(s, ExperimentId(0), this);
  connect(qs, &QSelection::onSelect,
          this, &QTexturesModel::onSelect);
  emit onTexturesChanged();
}

QQmlListProperty<QTexturesValue>
QTexturesModel::textures() {
  return QQmlListProperty<QTexturesValue>(this, m_textures_list);
}

void
QTexturesModel::onTextures(RenderId renderId,
                     const std::vector<TextureData> &textures) {
  /*if (m_textures.find(texturesData.textures) == m_textures.end())
    // the textures groups have many duplicates.  The textures prefers to
    // use the smallest group with a textures of the same name.
    return;

  float value = 0;
  for (auto v : texturesData.data)
    value += v;
  if (selectionCount == SelectionId(0))
    m_textures[texturesData.textures]->setFrameValue(value);
  else
    m_textures[texturesData.textures]->setValue(value);*/
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
