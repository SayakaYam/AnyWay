// 2020/10/21
// 制作者：山田里矢香
// 概要：Player挙動
// 2021/10/21:移動処理・ステート管理
// 2021/10/22:ジャンプ処理

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

private:	// CharaPlayer内の関数--------------------------
	void BitStateControl(float _deltaTime);			// ステート管理
	void UpdatePlayerMove(float _deltaTime);		// 移動処理
	void PlayerJump(float _deltaTime);				// ジャンプ処理
	// -----------------------------------------------------

private:	// 入力バインドの関数---------------------------
	void PlayerMoveForward(float _axisValue);		// プレイヤーの上下処理
	void PlayerMoveRight(float _axisValue);			// プレイヤーの左右処理
	void PlayerJumpStart()				;			// ジャンプ入力処理
	// -----------------------------------------------------

private:	// CharaPlayer内の変数--------------------------
	// ステート変数-----------------------------------------
	const unsigned int BIT_FLAG_WAIT = (1 << 0);	// 待機ステート　　　 0000 0000
	const unsigned int BIT_FLAG_MOVE = (1 << 1);	// 移動ステート　　　 0000 0010
	const unsigned int BIT_FLAG_JUMP = (1 << 2);	// ジャンプステート　 0000 0100
	const unsigned int BIT_FLAG_SQUAT = (1 << 3);	// しゃがみステート　 0000 1000
	const unsigned int BIT_FLAG_CAMERA = (1 << 4);	// カメラ移動ステート 0001 0000

	UPROPERTY(EditAnywhere, Category = "PlayerStatus")
		unsigned int m_PlayerStatus;				// ステート管理用変数
	// -----------------------------------------------------
	// 移動変数-------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "PlayerMove")
		FVector2D m_PlayerMoveInput;				// キャラ移動入力用
	// -----------------------------------------------------
	// ジャンプ変数-----------------------------------------
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_PlayerJumpPawer;					// 基礎ジャンプ力
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_AirTimeCnt;							// 空中時間のカウント
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_GravityScale;						// 重力規模
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_NowJumpPawer;						// 今のジャンプ量
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_PrevJumpPawer;						// 前フレームのジャンプ量
	UPROPERTY(EditAnywhere, Category = "MyJump")
		float m_PrevLocationZ;						// 前フレームのZ軸
	UPROPERTY(EditAnywhere, Category = "MyJump")
		bool m_IsAir;								// 空中状態か
	UPROPERTY(EditAnywhere, Category = "MyJump")
		bool m_IsFalling;							// 落下中か
	// -----------------------------------------------------
	// -----------------------------------------------------
};
