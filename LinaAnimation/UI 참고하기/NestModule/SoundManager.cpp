#include "SoundManager.h"

// ���� ���̺귯�� �ʱ�ȭ
void SoundManager::Initialize()
{
	// �ý��� ����
	result = System_Create(&m_psystem);
	if (result != FMOD_OK) { return; }

	// �ý��� �ʱ�ȭ
	// ���ÿ� ��� ������ ä�� ��, �ý��� ���, ����Ʈ�� ����̹�(���⼭�� ������� ����)
	result = m_psystem->init(100, FMOD_INIT_NORMAL, nullptr);
	if(result != FMOD_OK) { return; }

	// ä�� �켱���� ����
	m_pbgmChannel->setPriority(0);
	m_psfxChannel->setPriority(1);

	// ��� �ʱ�ȭ
	m_masterVolume = 1.0f;
	m_bgmVolume = 1.0f;
	m_sfxVolume = 1.0f;

	m_isPlaying = false;
	m_isPaused = false;

	///
	std::cout << "SoundManager �ʱ�ȭ" << std::endl;
}

// �ý��� ������Ʈ -> ä�� ����
void SoundManager::Update()
{
	m_psystem->update();
}

// ���� �� ȣ���� �Լ�
// �Ҵ��� ���� ����, �ý��� ����
void SoundManager::Finalize()
{
	for (int i = 0; i < static_cast<int>(NestSound::END); i++)
	{
		m_psound[i]->release();
	}
	m_psystem->close();
	m_psystem->release();
}

void SoundManager::LoadSounds()
{
	// BGM �ε��ϱ�
	for (int i = static_cast<int>(NestSound::BGMTITLE); i < static_cast<int>(NestSound::BGMGAMEOVER); i++)
	{
		const std::string& filePath = "Sound/" + m_soundList[i];
		result = m_psystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_psound[i]);
		if (result != FMOD_OK) 
		{
			std::cout << "BGM ���� ���� �ε� ����\n";
			return; 
		}
	}

	// SFX �ε��ϱ�
	for (int i = static_cast<int>(NestSound::EFFECT01); i < static_cast<int>(NestSound::EFFECT04); i++)
	{
		const std::string& filePath = "Sound/" + m_soundList[i];
		result = m_psystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_psound[i]);
		if (result != FMOD_OK) 
		{
			std::cout << "SFX ���� ���� �ε� ����\n";
			return; 
		}
	}
}

// �׽�Ʈ�� �Լ� - ���� ���� �ϳ��� �о�´�.
void SoundManager::LoadSound(std::string _fileName)
{
	const std::string& filePath = "../NestResources/Sound/" + _fileName;
	result = m_psystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_psound[static_cast<int>(NestSound::ETC01)]);
	if (result != FMOD_OK) 
	{
		std::cout << "�׽�Ʈ�� �Լ� [LoadSound] ���� �ε� ����\n";
		return; 
	}
}

// ���� ���
void SoundManager::PlayBGM(NestSound _nestSound)
{
	if (m_isPlaying == false)
	{
		if (m_isPaused) { m_isPaused = false; }
		result = m_psystem->playSound(m_psound[static_cast<int>(_nestSound)], m_bgmChannelGroup, false, &m_pbgmChannel);
		m_pbgmChannel->setVolume(m_bgmChannelVolume);
		if (result != FMOD_OK) { return; }

		m_isPlaying = true;
	}
}

// ���� ����
void SoundManager::StopBGM()
{
	result = m_pbgmChannel->stop();
	if (result != FMOD_OK) { return; }
	m_isPlaying = false;
}

// ���� �Ͻ�����
void SoundManager::PauseBGM()
{
	if (m_pbgmChannel)
	{
		if (m_isPaused)
		{
			m_pbgmChannel->setPaused(false);	// �Ͻ����� ����
			m_isPaused = false;
		}
		else
		{
			m_pbgmChannel->setPaused(true);		// �Ͻ�����
			m_isPaused = true;
		}
	}
}

// SFX ����
void SoundManager::PlaySFX(NestSound _nestSound)
{
	result = m_psystem->playSound(m_psound[static_cast<int>(_nestSound)], m_sfxChannelGroup, false, &m_psfxChannel);
	m_psfxChannel->setVolume(m_sfxChannelVolume);
	if (result != FMOD_OK) { return; }
}

// SFX ����
void SoundManager::StopSFX()
{
	result = m_psfxChannel->stop();
	if (result != FMOD_OK) { return; }
}

// ������ ���� ���� - amount ��ŭ���� ������ �����ϰ� �����ؾ� �Ѵ�.
void SoundManager::AdjustMasterVolume(float _amount)
{
	float& targetVolume = m_masterVolume;

	targetVolume = _amount;
	if (targetVolume > 1.0f) { targetVolume = 1.0f; } // �ִ� ���� üũ	
	if (targetVolume < 0.0f) { targetVolume = 0.0f; } // �ּ� ���� üũ

	m_bgmChannelVolume = m_masterVolume * m_bgmVolume;
	m_sfxChannelVolume = m_masterVolume * m_sfxVolume;

	m_pbgmChannel->setVolume(m_bgmChannelVolume);
	m_psfxChannel->setVolume(m_sfxChannelVolume);
}

// Ư�� ä���� ������ �����ϴ� �Լ�
void SoundManager::AdjustChannelVolume(ChannelType _channelType, float _amount)
{
	float& targetVolume = (_channelType == ChannelType::BGM) ? m_bgmVolume : m_sfxVolume;
	if (targetVolume > 1.0f) { targetVolume = 1.0f; } // �ִ� ���� üũ	
	if (targetVolume < 0.0f) { targetVolume = 0.0f; } // �ּ� ���� üũ

	targetVolume = _amount;
	m_bgmChannelVolume = m_masterVolume * m_bgmVolume;
	m_sfxChannelVolume = m_masterVolume * m_sfxVolume;

	if (_channelType == ChannelType::BGM)
	{
		m_pbgmChannel->setVolume(m_bgmChannelVolume);
	}
	
	else
	{
		m_psfxChannel->setVolume(m_sfxChannelVolume);
	}
}

// ä�� ������ ���� �����ϴ� �Լ�
void SoundManager::SetVolumeDirectly(FMOD::Channel* _channel, float _amount)
{
	_channel->setVolume(_amount);
}
