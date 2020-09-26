#pragma once
#include <string>
#include <glm/glm.hpp>

void DebugLog(const std::string &message);
void DebugLog(const std::string &message, const float &f);
void DebugLog(const std::string &message, const glm::vec3 &v);
