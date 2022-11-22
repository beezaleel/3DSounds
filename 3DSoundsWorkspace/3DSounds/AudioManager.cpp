#include "AudioManager.h"

// Default constructor
AudioManager::AudioManager() :fmodSystem_(nullptr) {}

// Destructor
AudioManager::~AudioManager() {}

bool AudioManager::isOkay(const FMOD_RESULT& result, bool displayError)
{
	if (result == FMOD_OK)
	{
		return true;
	}

	if (displayError)
	{
		printf("%d - %s", result, FMOD_ErrorString(result));
	}

	return false;
}

bool AudioManager::Initialize(const InitializationSettings& settings)
{
	if (!isOkay(FMOD::System_Create(&fmodSystem_)))
	{
		return false;
	}

	if (!isOkay(fmodSystem_->init(settings.maxChannels, FMOD_INIT_NORMAL, nullptr)))
	{
		return false;
	}

	return true;
}

void AudioManager::Shutdown()
{
	for (const auto& channelGroup : channelGroups_)
	{
		channelGroup.second->release();
	}
	channelGroups_.clear();

	for (const auto& sound : sounds_)
	{
		sound.second->release();
	}
	sounds_.clear();

	if (fmodSystem_)
	{
		isOkay(fmodSystem_->release());
	}
	fmodSystem_ = nullptr;

	printf("Audio manager shutdown successfully\n");
}

bool AudioManager::LoadSound(const std::string& name, const std::string& path, int flags)
{
	assert(fmodSystem_ && "no system object");
	assert(sounds_.find(name) == sounds_.end() && "sound already loaded");

	FMOD::Sound* sound;
	if (!isOkay(fmodSystem_->createSound(path.c_str(), flags, nullptr, &sound)))
	{
		return false;
	}

	sounds_.emplace(name, sound);

	return true;
}

bool AudioManager::UnloadSound(const std::string& name)
{
	assert(fmodSystem_ && "no system object");
	assert(sounds_.find(name) != sounds_.end() && "sound not found");

	const auto sound = sounds_.find(name);
	if (sound == sounds_.end())
	{
		return false;
	}

	sound->second->release();
	sounds_.erase(sound);

	return true;
}

bool AudioManager::PlaySound(const std::string& name)
{
	assert(fmodSystem_ && "no system object");
	assert(sounds_.find(name) != sounds_.end() && "sound not found");

	const auto sound = sounds_.find(name);
	if (sound == sounds_.end())
	{
		return false;
	}

	FMOD::Channel* channel;
	if (!isOkay(fmodSystem_->playSound(sound->second, nullptr, false, &channel)))
	{
		return false;
	}

	playingChannels_.emplace_back(channel);

	return true;
}

bool AudioManager::PlaySound(const std::string& soundName, const std::string& channelGroupName)
{
	assert(fmodSystem_ && "no system object");

	assert(sounds_.find(soundName) != sounds_.end() && "sound not found");
	const auto sound = sounds_.find(soundName);
	if (sound == sounds_.end())
	{
		return false;
	}

	assert(channelGroups_.find(channelGroupName) != channelGroups_.end() && "channel group not found");
	const auto channelGroup = channelGroups_.find(channelGroupName);
	if (channelGroup == channelGroups_.end())
	{
		return false;
	}

	FMOD::Channel* channel;
	if (!isOkay(fmodSystem_->playSound(sound->second, channelGroup->second, false, &channel)))
	{
		return false;
	}

	playingChannels_.emplace_back(channel);

	return true;
}

bool AudioManager::tick(const glm::vec3& cameraPosition)
{
	assert(fmodSystem_ && "no system object");

	FMOD_VECTOR fmodCameraPosition;
	fmodCameraPosition.x = cameraPosition.x;
	fmodCameraPosition.y = cameraPosition.y;
	fmodCameraPosition.z = cameraPosition.z;

	isOkay(fmodSystem_->set3DListenerAttributes(0, &fmodCameraPosition, nullptr, nullptr, nullptr));

	return isOkay(fmodSystem_->update());
}

bool AudioManager::LoadChannelGroup(const std::string& name)
{
	assert(fmodSystem_ && "no system object");
	FMOD::ChannelGroup* channelGroup;

	if (!isOkay(fmodSystem_->createChannelGroup(name.c_str(), &channelGroup)))
	{
		return false;
	}

	channelGroups_.emplace(name, channelGroup);

	return true;
}

bool AudioManager::UnloadChannelGroup(const std::string& name)
{
	assert(fmodSystem_ && "no system object");
	assert(channelGroups_.find(name) != channelGroups_.end() && "channel group not found");
	const auto channelGroup = channelGroups_.find(name);
	if (channelGroup == channelGroups_.end())
	{
		return false;
	}

	if (!isOkay(channelGroup->second->release()))
	{
		return false;
	}

	channelGroups_.erase(channelGroup);

	return true;
}

bool AudioManager::SetChannelGroupParent(const std::string& childName, const std::string& parentName)
{
	assert(fmodSystem_ && "no system object");
	assert(channelGroups_.find(childName) != channelGroups_.end() && "child channel group not found");
	assert(channelGroups_.find(parentName) != channelGroups_.end() && "parent channel group not found");

	const auto childChannelGroup = channelGroups_.find(childName);
	const auto parentChannelGroup = channelGroups_.find(parentName);

	if (childChannelGroup == channelGroups_.end() || parentChannelGroup == channelGroups_.end())
	{
		return false;
	}

	return isOkay(parentChannelGroup->second->addGroup(childChannelGroup->second));
}

bool AudioManager::GetChannelGroupVolume(const std::string& name, float** volume)
{
	assert(fmodSystem_ && "no system object");
	assert(channelGroups_.find(name) != channelGroups_.end() && "channel group not found");
	const auto channelGroup = channelGroups_.find(name);
	if (channelGroup == channelGroups_.end())
	{
		return false;
	}

	return isOkay(channelGroup->second->getVolume(*volume));
}

bool AudioManager::SetChannelGroupVolume(const std::string& name, float volume)
{
	assert(fmodSystem_ && "no system object");
	assert(channelGroups_.find(name) != channelGroups_.end() && "channel group not found");
	const auto channelGroup = channelGroups_.find(name);
	if (channelGroup == channelGroups_.end())
	{
		return false;
	}

	return isOkay(channelGroup->second->setVolume(volume));
}

bool AudioManager::SetListenerPosition(const glm::vec3 position)
{
	FMOD_VECTOR fmodPosition;
	fmodPosition.x = position.x;
	fmodPosition.y = position.y;
	fmodPosition.z = position.z;

	return isOkay(fmodSystem_->set3DListenerAttributes(0, &fmodPosition, nullptr, nullptr, nullptr));
}

bool AudioManager::PlaySound(const std::string& soundName, glm::vec3 position, float maxDistance, FMOD::Channel** channel)
{
	assert(fmodSystem_ && "no system object");

	assert(sounds_.find(soundName) != sounds_.end() && "sound not found");
	const auto sound = sounds_.find(soundName);
	if (sound == sounds_.end())
	{
		return false;
	}

	if (!isOkay(fmodSystem_->playSound(sound->second, nullptr, true, channel)))
	{
		return false;
	}

	FMOD_VECTOR fmodSoundPosition;
	fmodSoundPosition.x = position.x;
	fmodSoundPosition.y = position.y;
	fmodSoundPosition.z = position.z;

	if (!isOkay((*channel)->set3DAttributes(&fmodSoundPosition, nullptr)))
	{
		return false;
	}

	//min distance to hear @ max volume
	//max distance where the attenuation stops
	if (!isOkay((*channel)->set3DMinMaxDistance(maxDistance, 10000.0f)))
	{
		return false;
	}

	if (!isOkay((*channel)->setPaused(false)))
	{
		return false;
	}

	return true;
}

bool AudioManager::Update3dSoundPosition(FMOD::Channel* channel, const glm::vec3 position)
{
	FMOD_VECTOR fmodPosition;
	fmodPosition.x = position.x;
	fmodPosition.y = position.y;
	fmodPosition.z = position.z;
	return isOkay(channel->set3DAttributes(&fmodPosition, nullptr));
}

bool AudioManager::UpdateSoundVolume(FMOD::Channel* channel, const float newVolume)
{
	return isOkay(channel->setVolume(newVolume));
}
