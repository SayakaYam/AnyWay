// 2020/10/21
// 制作者：山田里矢香
// 概要：Player挙動制御
// 2021/10/21:移動処理・ステート管理
// 2021/10/22:ジャンプ処理

#include "Chara_Player.h"
#include "Components/InputComponent.h"

// defineマクロ
#define RISE_HEIGHT (m_AirTimeCnt * m_AirTimeCnt * (-m_GravityScale) / 2) + (m_AirTimeCnt * m_PlayerJumpPawer)	// ジャンプ計算用
#define FALL_HEIGHT (m_AirTimeCnt * m_AirTimeCnt * (-m_GravityScale) / 2)										// 落下計算用
#define JUMP_MAXTIME 0.3f	// ジャンプ中の時間
// ifdef
#define MOVE
#define JUMP
#define DEBUG

// 初期化
AChara_Player::AChara_Player():
	m_PlayerStatus(0),
	// 移動------------------
	m_PlayerMoveInput(FVector2D::ZeroVector),
	// ----------------------
	// ジャンプ--------------
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

	// ステート管理
	BitStateControl(DeltaTime);			
}

// Called to bind functionality to input
void AChara_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (Controller != NULL) {
		// アクション入力-----------------------------------------------------------------------
		InputComponent->BindAction("PlayerJumpStart", IE_Pressed, this, &AChara_Player::PlayerJumpStart);
		// -------------------------------------------------------------------------------------
		// 軸入力-------------------------------------------------------------------------------
		InputComponent->BindAxis("PlayerMoveForward", this, &AChara_Player::PlayerMoveForward);
		InputComponent->BindAxis("PlayerMoveRight", this, &AChara_Player::PlayerMoveRight);
		// -------------------------------------------------------------------------------------
	}
}

// 毎フレーム処理関数---------------------------------------------------------------------------
void AChara_Player::BitStateControl(float _deltaTime)
{
	// ジャンプ中か:4
	if (m_PlayerStatus & BIT_FLAG_JUMP) { PlayerJump(_deltaTime); }
	// 移動中か:2
	else if (m_PlayerStatus & BIT_FLAG_MOVE) { UpdatePlayerMove(_deltaTime); }
	// 待機中か:0
	else if(m_PlayerStatus & BIT_FLAG_WAIT){ UE_LOG(LogTemp, Warning, TEXT("PlayerWait")); }
}
// ---------------------------------------------------------------------------------------------

// プレイヤー挙動-------------------------------------------------------------------------------
// 移動処理
void AChara_Player::UpdatePlayerMove(float _deltaTime)
{
#ifdef MOVE
	// 上下に移動
	const FVector ForwardVector = this->GetActorForwardVector();
	AddMovementInput(ForwardVector, m_PlayerMoveInput.Y * 2.0f);
	// 左右に移動
	const FVector RightVector = this->GetActorRightVector();
	AddMovementInput(RightVector, m_PlayerMoveInput.X * 2.0f);

	// 移動フラグを下げる
	m_PlayerStatus &= ~BIT_FLAG_MOVE;
#endif // MOVE

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("PlayerMove"));
#endif
}

// ジャンプ処理
void AChara_Player::PlayerJump(float _deltaTime)
{	
#ifdef JUMP
	m_AirTimeCnt += _deltaTime;		// 空中時間を加算
	FVector2D nowPos = FVector2D(this->GetActorLocation().X, this->GetActorLocation().Y);		// 現状の位置情報

	// 落下状態ではない場合
	if (!m_IsFalling) {
		m_NowJumpPawer = RISE_HEIGHT;	// ジャンプ計算

		// 今のジャンプ量が前フレームのジャンプ量より下がっていたら落下状態にする
		if (m_NowJumpPawer < m_PrevJumpPawer && m_NowJumpPawer>0.0f) {

			m_PrevLocationZ = this->GetActorLocation().Z;	// 前フレームとしてのZ軸を取得

			// 落下状態にする
			m_AirTimeCnt = 0.0f;			
			m_IsFalling = true;
		}
		else {	
			m_PrevJumpPawer = m_NowJumpPawer;	//今のジャンプ量を前フレームとして反映
		}
	}
	// 落下状態の場合
	else {
		m_NowJumpPawer = FALL_HEIGHT;	// 落下計算

		// ジャンプ時間外の場合
		if (m_AirTimeCnt > JUMP_MAXTIME) {

			// ジャンプフラグを下げる
			m_PlayerStatus &= ~BIT_FLAG_JUMP;
			m_IsAir = false;
		}
	}

	// 位置の反映
	SetActorLocation(FVector(nowPos.X, nowPos.Y, m_PrevLocationZ + m_NowJumpPawer));
#endif // JUMP

#ifdef DEBUG
	UE_LOG(LogTemp, Warning, TEXT("PlayerJump"));
#endif
}
// ---------------------------------------------------------------------------------------------

// 入力バインド---------------------------------------------------------------------------------
// 上下の入力
void AChara_Player::PlayerMoveForward(float _axisValue)
{
	// 入力が入っているか
	if (_axisValue != 0.0f) {
		m_PlayerStatus |= BIT_FLAG_MOVE;	// 移動フラグを立てる
		m_PlayerMoveInput.Y= FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5;	// -1.0～1.0の間の値を取得
	}
}
// 左右の入力
void AChara_Player::PlayerMoveRight(float _axisValue)
{
	// 入力が入っているか
	if (_axisValue != 0.0f) {
		m_PlayerStatus |= BIT_FLAG_MOVE;	// 移動フラグを立てる
		m_PlayerMoveInput.X= FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5;	// -1.0～1.0の間の値を取得
	}
}
// ジャンプ入力
void AChara_Player::PlayerJumpStart()
{
	// 空中状態なら処理しない
	if (m_IsAir)return;

	m_PlayerStatus |= BIT_FLAG_JUMP;	// ジャンプフラグを立てる
	m_PrevLocationZ = this->GetActorLocation().Z;	// 前フレームとしてのZ軸を取得

	// 変数の初期化
	m_AirTimeCnt = 0.0f;
	m_NowJumpPawer = 0.0f;
	m_PrevJumpPawer = 0.0f;
	m_IsFalling = false;
	m_IsAir = true;
}
// ---------------------------------------------------------------------------------------------


