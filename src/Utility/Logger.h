#pragma once
#include <glm/glm.hpp>
#include <string>

void DebugLog(const std::string& message);
void DebugLog(const std::string& message, const float& f);
void DebugLog(const std::string& message, const glm::vec3& v);
