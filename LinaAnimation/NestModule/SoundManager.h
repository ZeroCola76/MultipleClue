#pragma once
#include <vector>
#include <string>
#include "IManager.h"
#include "..\NestModule\core\inc\fmod.hpp"

///<summary>
/// 사운드 매니저
/// 사운드를 로드하고 관리한다.
/// Fmod를 사용하여 구현
/// </summary>

// 사운드 종류 설정한 enum문
enum NestSound
{
	BGMTITLE = 0,
	BGMSTAGE1,
	BGMSTAGE2,
	BMGBOSS,
	BGMENDING,
	BGMGAMEOVER,

	EFFECT01,
	EFFECT02,
	EFFECT03,
	EFFECT04,

	ETC01,

	END
};

// 채널 종류 설정 : 배경음악 / 효과음 두 가지
enum class ChannelType
{
	BGM,
	SFX
};

class SoundManager : public IManager
{
public:
	SoundManager() {}
	virtual ~SoundManager() {}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

public:
	// 사운드 로딩 관련 함수
	void LoadSounds();
	void LoadSound(std::string _fileName);

	// 사운드 재생 관련 함수
	void PlayBGM(NestSound);
	void StopBGM();
	void PauseBGM();
	void PlaySFX(NestSound);
	void StopSFX();

	// 사운드 볼륨 조절 함수
	void AdjustMasterVolume(float _amount);
	void AdjustChannelVolume(ChannelType _channelType, float _amount);
	float GetVolumeMaster() { return m_masterVolume; }
	float GetVolumeBGM() { return m_bgmVolume; }
	float GetVolumeSFX() { return m_sfxVolume; }
	void SetVolumeDirectly(FMOD::Channel* _ch, float _amount);

private:
	// FMOD를 돌리기 위해 필요한 애들
	FMOD::System* m_psystem;									// 시스템
	FMOD::Sound* m_psound[static_cast<int>(NestSound::END)];	// 사운드 객체 배열
	std::vector<FMOD::Channel*> m_channels;						// 채널 배열		
	FMOD::Channel* m_pbgmChannel;								// 배경음악 채널
	FMOD::Channel* m_psfxChannel;								// 효과음 채널
	FMOD::Channel* m_otherChannel;								// 기타 채널
	std::vector<FMOD::ChannelGroup*> m_channelGroups;			// 채널 그룹 배열
	FMOD::ChannelGroup* m_bgmChannelGroup;						// 배경음악 채널 그룹
	FMOD::ChannelGroup* m_sfxChannelGroup;						// 효과음 채널 그룹
	std::vector<std::string> m_soundList;						// 사운드 파일 이름 배열
	FMOD_RESULT result;											// FMOD 함수 결과값

private:
	// 사운드 볼륨
	float m_masterVolume = 0.1f;
	float m_bgmVolume = 0.1f;
	float m_sfxVolume = 0.1f;
	float m_bgmChannelVolume = m_bgmVolume * m_masterVolume;
	float m_sfxChannelVolume = m_sfxVolume * m_masterVolume;

	bool m_isPlaying;	// 사운드 재생 여부
	bool m_isPaused;	// 사운드 일시정지 여부
};