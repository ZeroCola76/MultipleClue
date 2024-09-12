#include "SoundManager.h"

// 사운드 라이브러리 초기화
void SoundManager::Initialize()
{
	// 시스템 생성
	result = System_Create(&m_psystem);
	if (result != FMOD_OK) { return; }

	// 시스템 초기화
	// 동시에 재생 가능한 채널 수, 시스템 모드, 엑스트라 드라이버(여기서는 사용하지 않음)
	result = m_psystem->init(100, FMOD_INIT_NORMAL, nullptr);
	if(result != FMOD_OK) { return; }

	// 채널 우선순위 설정
	m_pbgmChannel->setPriority(0);
	m_psfxChannel->setPriority(1);

	// 멤버 초기화
	m_masterVolume = 1.0f;
	m_bgmVolume = 1.0f;
	m_sfxVolume = 1.0f;

	m_isPlaying = false;
	m_isPaused = false;

	///
	std::cout << "SoundManager 초기화" << std::endl;
}

// 시스템 업데이트 -> 채널 재사용
void SoundManager::Update()
{
	m_psystem->update();
}

// 종료 시 호출할 함수
// 할당한 사운드 해제, 시스템 종료
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
	// BGM 로드하기
	for (int i = static_cast<int>(NestSound::BGMTITLE); i < static_cast<int>(NestSound::BGMGAMEOVER); i++)
	{
		const std::string& filePath = "Sound/" + m_soundList[i];
		result = m_psystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_psound[i]);
		if (result != FMOD_OK) 
		{
			std::cout << "BGM 사운드 파일 로드 실패\n";
			return; 
		}
	}

	// SFX 로드하기
	for (int i = static_cast<int>(NestSound::EFFECT01); i < static_cast<int>(NestSound::EFFECT04); i++)
	{
		const std::string& filePath = "Sound/" + m_soundList[i];
		result = m_psystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_psound[i]);
		if (result != FMOD_OK) 
		{
			std::cout << "SFX 사운드 파일 로드 실패\n";
			return; 
		}
	}
}

// 테스트용 함수 - 사운드 파일 하나만 읽어온다.
void SoundManager::LoadSound(std::string _fileName)
{
	const std::string& filePath = "../NestResources/Sound/" + _fileName;
	result = m_psystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_psound[static_cast<int>(NestSound::ETC01)]);
	if (result != FMOD_OK) 
	{
		std::cout << "테스트용 함수 [LoadSound] 파일 로드 실패\n";
		return; 
	}
}

// 사운드 재생
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

// 사운드 정지
void SoundManager::StopBGM()
{
	result = m_pbgmChannel->stop();
	if (result != FMOD_OK) { return; }
	m_isPlaying = false;
}

// 사운드 일시정지
void SoundManager::PauseBGM()
{
	if (m_pbgmChannel)
	{
		if (m_isPaused)
		{
			m_pbgmChannel->setPaused(false);	// 일시정지 해제
			m_isPaused = false;
		}
		else
		{
			m_pbgmChannel->setPaused(true);		// 일시정지
			m_isPaused = true;
		}
	}
}

// SFX 실행
void SoundManager::PlaySFX(NestSound _nestSound)
{
	result = m_psystem->playSound(m_psound[static_cast<int>(_nestSound)], m_sfxChannelGroup, false, &m_psfxChannel);
	m_psfxChannel->setVolume(m_sfxChannelVolume);
	if (result != FMOD_OK) { return; }
}

// SFX 정지
void SoundManager::StopSFX()
{
	result = m_psfxChannel->stop();
	if (result != FMOD_OK) { return; }
}

// 마스터 볼륨 조절 - amount 만큼으로 볼륨을 설정하게 수정해야 한다.
void SoundManager::AdjustMasterVolume(float _amount)
{
	float& targetVolume = m_masterVolume;

	targetVolume = _amount;
	if (targetVolume > 1.0f) { targetVolume = 1.0f; } // 최대 볼륨 체크	
	if (targetVolume < 0.0f) { targetVolume = 0.0f; } // 최소 볼륨 체크

	m_bgmChannelVolume = m_masterVolume * m_bgmVolume;
	m_sfxChannelVolume = m_masterVolume * m_sfxVolume;

	m_pbgmChannel->setVolume(m_bgmChannelVolume);
	m_psfxChannel->setVolume(m_sfxChannelVolume);
}

// 특정 채널의 볼륨을 조절하는 함수
void SoundManager::AdjustChannelVolume(ChannelType _channelType, float _amount)
{
	float& targetVolume = (_channelType == ChannelType::BGM) ? m_bgmVolume : m_sfxVolume;
	if (targetVolume > 1.0f) { targetVolume = 1.0f; } // 최대 볼륨 체크	
	if (targetVolume < 0.0f) { targetVolume = 0.0f; } // 최소 볼륨 체크

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

// 채널 볼륨을 직접 조절하는 함수
void SoundManager::SetVolumeDirectly(FMOD::Channel* _channel, float _amount)
{
	_channel->setVolume(_amount);
}
