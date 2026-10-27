#ifndef MARX_VECTOR_IMPLEMENTATION
#define MARX_VECTOR_IMPLEMENTATION

#include <ostream>

namespace marx
{

struct iVector2
{
  int x, y;

  constexpr iVector2 operator+(const iVector2 &Other) const
  {
    return { x + Other.x, y + Other.y };
  }

  constexpr iVector2 operator-(const iVector2 &Other) const
  {
    return { x - Other.x, y - Other.y };
  }

  constexpr iVector2 operator*(const iVector2 &Other) const
  {
    return { x * Other.x, y * Other.y };
  }

  constexpr iVector2 operator/(const iVector2 &Other) const
  {
    return { x / Other.x, y / Other.y };
  }

  constexpr iVector2 &operator+=(const iVector2 &Other)
  {
    x += Other.x;
    y += Other.y;
    return *this;
  }

  constexpr iVector2 &operator-=(const iVector2 &Other)
  {
    x -= Other.x;
    y -= Other.y;
    return *this;
  }

  constexpr iVector2 &operator*=(const iVector2 &Other)
  {
    x *= Other.x;
    y *= Other.y;
    return *this;
  }

  constexpr iVector2 &operator/=(const iVector2 &Other)
  {
    x /= Other.x;
    y /= Other.y;
    return *this;
  }

  constexpr iVector2 operator+() const
  {
    return *this;
  }

  constexpr iVector2 operator-() const
  {
    return { -x, -y };
  }

  constexpr iVector2 &operator++()
  {
    ++x;
    ++y;
    return *this;
  }

  constexpr iVector2 operator++(int)
  {
    iVector2 Previous = *this;
    ++(*this);
    return Previous;
  }

  constexpr iVector2 &operator--()
  {
    --x;
    --y;
    return *this;
  }

  constexpr iVector2 operator--(int)
  {
    iVector2 Previous = *this;
    --(*this);
    return Previous;
  }

  constexpr bool operator==(const iVector2 &Other) const
  {
    return x == Other.x && y == Other.y;
  }

  constexpr bool operator!=(const iVector2 &Other) const
  {
    return !(*this == Other);
  }

  constexpr bool operator>(const iVector2 &Other) const
  {
    return x > Other.x && y > Other.y;
  }

  constexpr bool operator<(const iVector2 &Other) const
  {
    return x < Other.x && y < Other.y;
  }

  constexpr bool operator>=(const iVector2 &Other) const
  {
    return x >= Other.x && y >= Other.y;
  }

  constexpr bool operator<=(const iVector2 &Other) const
  {
    return x <= Other.x && y <= Other.y;
  }

  constexpr iVector2(int x = 0, int y = 0)
      : x(x)
      , y(y)
  {
  }
};

struct fVector2
{
  float x, y;

  constexpr fVector2 operator+(const fVector2 &Other) const
  {
    return { x + Other.x, y + Other.y };
  }

  constexpr fVector2 operator-(const fVector2 &Other) const
  {
    return { x - Other.x, y - Other.y };
  }

  constexpr fVector2 operator*(const fVector2 &Other) const
  {
    return { x * Other.x, y * Other.y };
  }
  constexpr fVector2 operator*(float Scalar) const
  {
    return { x * Scalar, y * Scalar };
  }

  constexpr fVector2 operator/(const fVector2 &Other) const
  {
    return { x / Other.x, y / Other.y };
  }
  constexpr fVector2 operator/(float Scalar) const
  {
    return { x / Scalar, y / Scalar };
  }

  constexpr fVector2 &operator+=(const fVector2 &Other)
  {
    x += Other.x;
    y += Other.y;
    return *this;
  }

  constexpr fVector2 &operator-=(const fVector2 &Other)
  {
    x -= Other.x;
    y -= Other.y;
    return *this;
  }

  constexpr fVector2 &operator*=(const fVector2 &Other)
  {
    x *= Other.x;
    y *= Other.y;
    return *this;
  }

  constexpr fVector2 &operator/=(const fVector2 &Other)
  {
    x /= Other.x;
    y /= Other.y;
    return *this;
  }

  constexpr fVector2 operator+() const
  {
    return *this;
  }

  constexpr fVector2 operator-() const
  {
    return { -x, -y };
  }

  constexpr fVector2 &operator++()
  {
    ++x;
    ++y;
    return *this;
  }

  constexpr fVector2 operator++(int)
  {
    fVector2 Previous = *this;
    ++(*this);
    return Previous;
  }

  constexpr fVector2 &operator--()
  {
    --x;
    --y;
    return *this;
  }

  constexpr fVector2 operator--(int)
  {
    fVector2 Previous = *this;
    --(*this);
    return Previous;
  }

  constexpr bool operator==(const fVector2 &Other) const
  {
    return x == Other.x && y == Other.y;
  }

  constexpr bool operator!=(const fVector2 &Other) const
  {
    return !(*this == Other);
  }

  constexpr bool operator>(const fVector2 &Other) const
  {
    return x > Other.x && y > Other.y;
  }

  constexpr bool operator<(const fVector2 &Other) const
  {
    return x < Other.x && y < Other.y;
  }

  constexpr bool operator>=(const fVector2 &Other) const
  {
    return x >= Other.x && y >= Other.y;
  }

  constexpr bool operator<=(const fVector2 &Other) const
  {
    return x <= Other.x && y <= Other.y;
  }

  constexpr fVector2(float x = 0.0f, float y = 0.0f)
      : x(x)
      , y(y)
  {
  }
};

std::ostream &operator<<(std::ostream &Os, const iVector2 &Vector);
std::ostream &operator<<(std::ostream &Os, const fVector2 &Vector);
} // namespace marx

#endif
