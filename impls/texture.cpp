#include "../includes/texture.hpp"

namespace GeoFrame {
Tag Texture::sTag = ResourceBase::sTag + Tag("Texture");

void Texture::SetFilter(FilterType const &filter) {
  mFilter = filter;
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (unsigned)mFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (unsigned)mFilter);
  Unbind();
}

void Texture::SetWrapping(WrappingType const &wrap) {
  mWrap = wrap;
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (unsigned)mWrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (unsigned)mWrap);
  Unbind();
}

void Texture::LoadData(unsigned width, unsigned height, unsigned channels,
                       unsigned char const *data) {
  mWidth = width;
  mHeight = height;
  mChannels = channels;
  unsigned format;
  switch (mChannels) {
  case 1:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    format = GL_RED;
    break;
  case 2:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    format = GL_RG;
    break;
  case 3:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  default:
    format = 0;
    break;
  }

  if (format == 0) {
    M_GEO_THROW(KernelError, "Invalid number of channels.");
    return;
  }

  Bind();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, format,
               GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  SetFilter(mFilter);
  SetWrapping(mWrap);
  Unbind();
}

TextureData Texture::LoadTexture(Str const &path) {
  TextureData texData;
  stbi_set_flip_vertically_on_load(true);
  auto pixelData =
      stbi_load(path.c_str(), (int *)&texData.width, (int *)&texData.height,
                (int *)&texData.channels, 0);
  stbi_set_flip_vertically_on_load(false);

  if (pixelData == nullptr) {
    M_GEO_THROW(KernelError, "Failed to load texture.");
    return texData;
  }

  texData.pixels = Vec<unsigned char>(
      pixelData, pixelData + texData.width * texData.height * texData.channels);
  stbi_image_free(pixelData);
  return texData;
}

void CubeTexture::SetFilter(FilterType const &filter) {
  mFilter = filter;
  Bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  (unsigned)mFilter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                  (unsigned)mFilter);
  Unbind();
}

void CubeTexture::SetWrapping(WrappingType const &wrap) {
  mWrap = wrap;
  Bind();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (unsigned)mWrap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (unsigned)mWrap);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (unsigned)mWrap);
  Unbind();
}

void CubeTexture::LoadData(CubeFace face, unsigned width, unsigned height,
                           unsigned channels, unsigned char const *data) {
  mWidth = width;
  mHeight = height;
  mChannels = channels;
  unsigned format;
  switch (mChannels) {
  case 1:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    format = GL_RED;
    break;
  case 2:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    format = GL_RG;
    break;
  case 3:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
  default:
    format = 0;
    break;
  }

  if (format == 0) {
    M_GEO_THROW(KernelError, "Invalid number of channels.");
    return;
  }

  Bind();
  glTexImage2D((unsigned)face, 0, (unsigned)GL_RGBA, mWidth, mHeight, 0, format,
               GL_UNSIGNED_BYTE, data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  SetFilter(mFilter);
  SetWrapping(mWrap);
  Unbind();
}

void CubeTexture::LoadDatas(Vec<TextureData> const &datas) {
  for (unsigned i = 0; i < datas.size(); ++i) {
    this->LoadData((CubeFace)((unsigned)CubeFace::RIGHT + i), datas[i]);
  }
}
} // namespace GeoFrame
