#include "Ray.h"

// Raycasting reference
// https://github.com/Hopson97/MineCraft-One-Week-Challenge/blob/master/Source/Maths/Ray.cpp

Ray::Ray(const glm::vec3 & position, const glm::vec3 & direction)
    : m_rayStart(position)
    , m_rayEnd(position)
    , m_direction(direction)
{
}

void Ray::step(float scale)
{
    float yaw   = glm::radians(m_direction.y + 90); // no clue why + 90
    float pitch = glm::radians(m_direction.x);

    m_rayEnd.x -= glm::cos(yaw) * glm::cos(pitch) * scale;
    m_rayEnd.z -= glm::sin(yaw) * glm::cos(pitch) * scale;
    m_rayEnd.y -= glm::sin(pitch)                 * scale;

    // Moguce samo podijeliti cijeli vektor sa glm::cos(pitch) pa se dobije drugacija formula
    /*  
        m_rayEnd.x -= glm::cos(yaw)   * scale;
        m_rayEnd.z -= glm::sin(yaw)   * scale;
        m_rayEnd.y -= glm::tan(pitch) * scale;
    */
}

const glm::vec3 & Ray::getEnd() const
{
    return m_rayEnd;
}

float Ray::getLength() const
{
    return glm::distance(m_rayStart, m_rayEnd);
}
