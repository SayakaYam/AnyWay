// 2020/10/21
// ����ҁF�R�c���
// �T�v�FPlayer����
// 2021/10/21:�ړ������E�X�e�[�g�Ǘ�
// 2021/10/22:�W�����v����

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Chara_Player.generated.h"

UCLASS()
class ANYWAY_API AChara_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChara_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:	// CharaPlayer���̊֐�--------------------------
	void BitStateControl(float _deltaTime);			// �X�e�[�g�Ǘ�
	void UpdatePlayerMove(float _deltaTime);		// �ړ�����
	void PlayerJump(float _deltaTime);				// �W�����v����
	// -----------------------------------------------------

private:	// ���̓o�C���h�̊֐�---------------------------
	void PlayerMoveForward(float _axisValue);		// �v���C���[�̏㉺����
	void PlayerMoveRight(float _axisValue);			// �v���C���[�̍��E����
	void PlayerJumpStart()				;			// �W�����v���͏���
	// -----------------------------------------------------

private:	// CharaPlayer���̕ϐ�--------------------------
	// �X�e�[�g�ϐ�-----------------------------------------
	const unsigned int BIT_FLAG_WAIT = (1 << 0);	// �ҋ@�X�e�[�g�@�@�@ 0000 0000
	const unsigned int BIT_FLAG_MOVE = (1 << 1);	// �ړ��X�e�[�g�@�@�@ 0000 0010
	const unsigned int BIT_FLAG_JUMP = (1 << 2);	// �W�����v�X�e�[�g�@ 0000 0100
	const unsigned int BIT_FLAG_SQUAT = (1 << 3);	// ���Ⴊ�݃X�e�[�g�@ 0000 1000
	const unsigned int BIT_FLAG_CAMERA = (1 << 4);	// �J�����ړ��X�e�[�g 0001 0000

	UPROPERTY(EditAnywhere, Category = "PlayerStatus")
		unsigned int m_PlayerStatus;				// �X�e�[�g�Ǘ��p�ϐ�
	// -----------------------------------------------------
	// �ړ��ϐ�-------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "PlayerMove")
		FVector2D m_PlayerMoveInput;				// �L�����ړ����͗p
	// -----------------------------------------------------
	// �W�����v�ϐ�-----------------------------------------
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_PlayerJumpPawer;					// ��b�W�����v��
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_AirTimeCnt;							// �󒆎��Ԃ̃J�E���g
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_GravityScale;						// �d�͋K��
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_NowJumpPawer;						// ���̃W�����v��
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_PrevJumpPawer;						// �O�t���[���̃W�����v��
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_PrevLocationZ;						// �O�t���[����Z��
	UPROPERTY(EditAnywhere, Category = "MyJump")
		bool m_IsAir;								// �󒆏�Ԃ�
	UPROPERTY(EditAnywhere, Category = "MyJump")
		bool m_IsFalling;							// ��������
	// -----------------------------------------------------
	// -----------------------------------------------------
};
