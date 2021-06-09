#include <Utility/Logger.h>

void DebugLog(const std::string& message)
{
    printf("%s\n", message.c_str());
}

void DebugLog(const std::string& message, const float& f)
{
    printf("%s: %f\n", message.c_str(), f);
}

void DebugLog(const std::string& message, const glm::vec3& v)
{
    printf("%s: (%f,%f,%f)\n", message.c_str(), v.x, v.y, v.z);
}
