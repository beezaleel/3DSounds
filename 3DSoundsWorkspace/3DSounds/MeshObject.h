#pragma once

#include <string>
#include <glm/glm.hpp>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

class MeshObject
{
public:
	MeshObject();
	~MeshObject();
	float scale;

	bool isWireframe;
	bool bUseRGBAColour;
	bool bDoNotLight;

	std::string meshName;
	std::string friendlyName;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec4 RGBAColour;
	glm::vec3 velocity;
	FMOD::Channel* attachedSound;
};
