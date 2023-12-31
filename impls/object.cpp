#include "../includes/object.hpp"

namespace GeoFrame {
std::mt19937 UUID::sRandomEngine = std::mt19937(std::random_device()());
Tag ResourceBase::sTag = Tag("Resource");

void UUID::GenerateUUID() {
  auto time = std::chrono::system_clock::now();
  time_t epoch = time.time_since_epoch().count();
  M_MEMCPY(mUUID, &epoch, sizeof(time_t));
  for (int i = 0; i < 2; i++) {
    unsigned random = sRandomEngine();
    M_MEMCPY(mUUID + sizeof(time_t) + i * sizeof(unsigned), &random,
             sizeof(unsigned));
  }
}

std::string UUID::ToString() const {
  std::stringstream ss;
  for (int i = 0; i < 8; i++) {
    short block = 0;
    memcpy(&block, mUUID + i * sizeof(short), sizeof(short));
    M_MEMCPY(&block, mUUID + i * sizeof(short), sizeof(short));
    ss << std::hex << std::setfill('0') << std::setw(4) << block;
    if (i != 7)
      ss << "-";
  }
  return ss.str();
}

Tag Tag::operator+=(Str const &other) {
  mTag += ".";
  mTag += other;
  return *this;
}

Tag Tag::operator+=(Tag const &other) {
  mTag += ".";
  mTag += other.mTag;
  return *this;
}

Tag &Tag::operator=(Str const &other) {
  mTag = other;
  return *this;
}

Tag &Tag::operator=(Tag const &other) {
  mTag = other.mTag;
  return *this;
}
} // namespace GeoFrame

std::ostream &operator<<(std::ostream &stream, const GeoFrame::UUID &uuid) {
  stream << uuid.ToString();
  return stream;
}
