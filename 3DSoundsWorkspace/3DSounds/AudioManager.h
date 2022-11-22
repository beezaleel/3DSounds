#pragma once

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

struct InitializationSettings
{
	int maxChannels;

	InitializationSettings(const int maxChannels = 512) : maxChannels(maxChannels) {}
};

class AudioManager
{
protected:
	FMOD::System* fmodSystem_;
	std::map<std::string, FMOD::Sound*> sounds_;
	std::map<std::string, FMOD::ChannelGroup*> channelGroups_;

	std::vector<FMOD::Channel*> playingChannels_;

	static bool isOkay(const FMOD_RESULT& result, bool displayError = true);
public:
	AudioManager();
	~AudioManager();
	bool Initialize(const InitializationSettings& settings = InitializationSettings());
	void Shutdown();

	bool LoadSound(const std::string& name, const std::string& path, int flags);
	bool UnloadSound(const std::string& name);
	bool PlaySound(const std::string& name);
	bool PlaySound(const std::string& soundName, const std::string& channelGroupName);
	bool tick(const glm::vec3& cameraPosition);

	bool LoadChannelGroup(const std::string& name);
	bool UnloadChannelGroup(const std::string& name);
	bool SetChannelGroupParent(const std::string& childName, const std::string& parentName);

	bool GetChannelGroupVolume(const std::string& name, float** volume) ;
	bool SetChannelGroupVolume(const std::string& name, float volume);

	bool SetListenerPosition(const glm::vec3 position);
	bool PlaySound(const std::string& soundName, glm::vec3 position, float maxDistance, FMOD::Channel** channel);
	bool Update3dSoundPosition(FMOD::Channel* channel, const glm::vec3 position);
	bool UpdateSoundVolume(FMOD::Channel* channel, const float newVolume);
};

