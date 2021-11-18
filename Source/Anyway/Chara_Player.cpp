// 2020/10/21
// ����ҁF�R�c���
// �T�v�FPlayer��������
// 2021/10/21:�ړ������E�X�e�[�g�Ǘ�
// 2021/10/22:�W�����v����

#include "Chara_Player.h"
#include "Components/InputComponent.h"

// define�}�N��
#define RISE_HEIGHT (m_AirTimeCnt * m_AirTimeCnt * (-m_GravityScale) / 2) + (m_AirTimeCnt * m_PlayerJumpPawer)	// �W�����v�v�Z�p
#define FALL_HEIGHT (m_AirTimeCnt * m_AirTimeCnt * (-m_GravityScale) / 2)										// �����v�Z�p
#define JUMP_MAXTIME 0.3f	// �W�����v���̎���
// ifdef
#define MOVE
#define JUMP
#define DEBUG

// ������
AChara_Player::AChara_Player():
	m_PlayerStatus(0),
	// �ړ�------------------
	m_PlayerMoveInput(FVector2D::ZeroVector),
	// ----------------------
	// �W�����v--------------
	m_PlayerJumpPawer(500.f),
	m_AirTimeCnt(0.0f),
	m_GravityScale(980.f),
	m_NowJumpPawer(0.0f),
	m_PrevJumpPawer(0.0f),
	m_PrevLocationZ(0.0f),
	m_IsAir(false),
	m_IsFalling(false)
	// ----------------------
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChara_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChara_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �X�e�[�g�Ǘ�
	BitStateControl(DeltaTime);			
}

// Called to bind functionality to input
void AChara_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (Controller != NULL) {
		// �A�N�V��������-----------------------------------------------------------------------
		InputComponent->BindAction("PlayerJumpStart", IE_Pressed, this, &AChara_Player::PlayerJumpStart);
		// -------------------------------------------------------------------------------------
		// ������-------------------------------------------------------------------------------
		InputComponent->BindAxis("PlayerMoveForward", this, &AChara_Player::PlayerMoveForward);
		InputComponent->BindAxis("PlayerMoveRight", this, &AChara_Player::PlayerMoveRight);
		// -------------------------------------------------------------------------------------
	}
}

// ���t���[�������֐�---------------------------------------------------------------------------
void AChara_Player::BitStateControl(float _deltaTime)
{
	// �W�����v����:4
	if (m_PlayerStatus & BIT_FLAG_JUMP) { PlayerJump(_deltaTime); }
	// �ړ�����:2
	else if (m_PlayerStatus & BIT_FLAG_MOVE) { UpdatePlayerMove(_deltaTime); }
	// �ҋ@����:0
	else if(m_PlayerStatus & BIT_FLAG_WAIT){ UE_LOG(LogTemp, Warning, TEXT("PlayerWait")); }
}
// ---------------------------------------------------------------------------------------------

// �v���C���[����-------------------------------------------------------------------------------
// �ړ�����
void AChara_Player::UpdatePlayerMove(float _deltaTime)
{
#ifdef MOVE
	// �㉺�Ɉړ�
	const FVector ForwardVector = this->GetActorForwardVector();
	AddMovementInput(ForwardVector, m_PlayerMoveInput.Y * 2.0f);
	// ���E�Ɉړ�
	const FVector RightVector = this->GetActorRightVector();
	AddMovementInput(RightVector, m_PlayerMoveInput.X * 2.0f);

	// �ړ��t���O��������
	m_PlayerStatus &= ~BIT_FLAG_MOVE;
#endif // MOVE

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("PlayerMove"));
#endif
}

// �W�����v����
void AChara_Player::PlayerJump(float _deltaTime)
{	
#ifdef JUMP
	m_AirTimeCnt += _deltaTime;		// �󒆎��Ԃ����Z
	FVector2D nowPos = FVector2D(this->GetActorLocation().X, this->GetActorLocation().Y);		// ����̈ʒu���

	// ������Ԃł͂Ȃ��ꍇ
	if (!m_IsFalling) {
		m_NowJumpPawer = RISE_HEIGHT;	// �W�����v�v�Z

		// ���̃W�����v�ʂ��O�t���[���̃W�����v�ʂ�艺�����Ă����痎����Ԃɂ���
		if (m_NowJumpPawer < m_PrevJumpPawer && m_NowJumpPawer>0.0f) {

			m_PrevLocationZ = this->GetActorLocation().Z;	// �O�t���[���Ƃ��Ă�Z�����擾

			// ������Ԃɂ���
			m_AirTimeCnt = 0.0f;			
			m_IsFalling = true;
		}
		else {	
			m_PrevJumpPawer = m_NowJumpPawer;	//���̃W�����v�ʂ�O�t���[���Ƃ��Ĕ��f
		}
	}
	// ������Ԃ̏ꍇ
	else {
		m_NowJumpPawer = FALL_HEIGHT;	// �����v�Z

		// �W�����v���ԊO�̏ꍇ
		if (m_AirTimeCnt > JUMP_MAXTIME) {

			// �W�����v�t���O��������
			m_PlayerStatus &= ~BIT_FLAG_JUMP;
			m_IsAir = false;
		}
	}

	// �ʒu�̔��f
	SetActorLocation(FVector(nowPos.X, nowPos.Y, m_PrevLocationZ + m_NowJumpPawer));
#endif // JUMP

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("PlayerJump"));
#endif
}
// ---------------------------------------------------------------------------------------------

// ���̓o�C���h---------------------------------------------------------------------------------
// �㉺�̓���
void AChara_Player::PlayerMoveForward(float _axisValue)
{
	// ���͂������Ă��邩
	if (_axisValue != 0.0f) {
		m_PlayerStatus |= BIT_FLAG_MOVE;	// �ړ��t���O�𗧂Ă�
		m_PlayerMoveInput.Y= FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5;	// -1.0�`1.0�̊Ԃ̒l���擾
	}
}
// ���E�̓���
void AChara_Player::PlayerMoveRight(float _axisValue)
{
	// ���͂������Ă��邩
	if (_axisValue != 0.0f) {
		m_PlayerStatus |= BIT_FLAG_MOVE;	// �ړ��t���O�𗧂Ă�
		m_PlayerMoveInput.X= FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5;	// -1.0�`1.0�̊Ԃ̒l���擾
	}
}
// �W�����v����
void AChara_Player::PlayerJumpStart()
{
	// �󒆏�ԂȂ珈�����Ȃ�
	if (m_IsAir)return;

	m_PlayerStatus |= BIT_FLAG_JUMP;	// �W�����v�t���O�𗧂Ă�
	m_PrevLocationZ = this->GetActorLocation().Z;	// �O�t���[���Ƃ��Ă�Z�����擾

	// �ϐ��̏�����
	m_AirTimeCnt = 0.0f;
	m_NowJumpPawer = 0.0f;
	m_PrevJumpPawer = 0.0f;
	m_IsFalling = false;
	m_IsAir = true;
}
// ---------------------------------------------------------------------------------------------


