#pragma once
#include <vector>
#include <string>
#include "IManager.h"
#include "..\NestModule\core\inc\fmod.hpp"

///<summary>
/// ���� �Ŵ���
/// ���带 �ε��ϰ� �����Ѵ�.
/// Fmod�� ����Ͽ� ����
/// </summary>

// ���� ���� ������ enum��
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

// ä�� ���� ���� : ������� / ȿ���� �� ����
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
	// ���� �ε� ���� �Լ�
	void LoadSounds();
	void LoadSound(std::string _fileName);

	// ���� ��� ���� �Լ�
	void PlayBGM(NestSound);
	void StopBGM();
	void PauseBGM();
	void PlaySFX(NestSound);
	void StopSFX();

	// ���� ���� ���� �Լ�
	void AdjustMasterVolume(float _amount);
	void AdjustChannelVolume(ChannelType _channelType, float _amount);
	float GetVolumeMaster() { return m_masterVolume; }
	float GetVolumeBGM() { return m_bgmVolume; }
	float GetVolumeSFX() { return m_sfxVolume; }
	void SetVolumeDirectly(FMOD::Channel* _ch, float _amount);

private:
	// FMOD�� ������ ���� �ʿ��� �ֵ�
	FMOD::System* m_psystem;									// �ý���
	FMOD::Sound* m_psound[static_cast<int>(NestSound::END)];	// ���� ��ü �迭
	std::vector<FMOD::Channel*> m_channels;						// ä�� �迭		
	FMOD::Channel* m_pbgmChannel;								// ������� ä��
	FMOD::Channel* m_psfxChannel;								// ȿ���� ä��
	FMOD::Channel* m_otherChannel;								// ��Ÿ ä��
	std::vector<FMOD::ChannelGroup*> m_channelGroups;			// ä�� �׷� �迭
	FMOD::ChannelGroup* m_bgmChannelGroup;						// ������� ä�� �׷�
	FMOD::ChannelGroup* m_sfxChannelGroup;						// ȿ���� ä�� �׷�
	std::vector<std::string> m_soundList;						// ���� ���� �̸� �迭
	FMOD_RESULT result;											// FMOD �Լ� �����

private:
	// ���� ����
	float m_masterVolume = 0.1f;
	float m_bgmVolume = 0.1f;
	float m_sfxVolume = 0.1f;
	float m_bgmChannelVolume = m_bgmVolume * m_masterVolume;
	float m_sfxChannelVolume = m_sfxVolume * m_masterVolume;

	bool m_isPlaying;	// ���� ��� ����
	bool m_isPaused;	// ���� �Ͻ����� ����
};