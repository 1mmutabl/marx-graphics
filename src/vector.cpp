#include "marx/graphics-internal/vector.hpp"

namespace marx
{
std::ostream &operator<<(std::ostream &Os, const iVector2 &Vector)
{
  return Os << "iVec2(" << Vector.x << ", " << Vector.y << ')';
}

std::ostream &operator<<(std::ostream &Os, const fVector2 &Vector)
{
  return Os << "fVec2(" << Vector.x << ", " << Vector.y << ')';
}

} // namespace marx
