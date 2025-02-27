// ScriptStruct M1.M1Request
// 0x0000 (0x0000 - 0x0000)
#pragma pack(push, 0x1)
struct alignas(0x01) FM1Request
{
};

// Enum M1Data.EM1EventBoostType
// NumValues: 0x0006
enum class EM1EventBoostType : uint8
{
	ItemRateBoost                            = 0,
	ItemAmountBoost                          = 1,
	CharExpBoost                             = 2,
	MasterExpBoost                           = 3,
	WeaponExpBoost                           = 4,
	EM1EventBoostType_MAX                    = 5,
};
// Enum M1Data.EM1BuffSubItemType
// NumValues: 0x000C
enum class EM1BuffSubItemType : uint8
{
	None                                     = 0,
	CharacterExpBoost                        = 1,
	WeaponProficiencyBoost                   = 2,
	GoldBoost                                = 3,
	QulipothiumBoost                         = 4,
	InventoryExpansion                       = 5,
	CharcterInvenExpansion                   = 6,
	ConsumableInvenExpansion                 = 7,
	WareHouseInvenExpansion                  = 8,
	PresetSlotExpansion                      = 9,
	SeasonExpBoost                           = 10,
	EM1BuffSubItemType_MAX                   = 11,
};

// Enum M1Data.EM1CurrencyInternalType
// NumValues: 0x000C
enum class EM1CurrencyInternalType : uint16
{
	None                                     = 0,
	Gold                                     = 1,
	BlackMana                                = 3,
	Qulipothium                              = 4,
	BattleCoin                               = 6,
	PlatinumCoin                             = 7,
	BonusCoin                                = 8,
	BlueCash_Paid                            = 111,
	BlueCash_Free                            = 112,
	RedCash_Paid                             = 121,
	RedCash_Free                             = 122,
	Max                                      = 131,
};

// Enum M1Data.EM1StatType
// NumValues: 0x00ED
enum class EM1StatType : uint8
{
	Stat_MaxHp                               = 0,
	Stat_KnockbackSpeed                      = 1,
	Stat_KnockbackTime                       = 2,
	Stat_KnockdownTime                       = 3,
	Stat_RangeMax                            = 4,
	Stat_SpreadSize_Deprecated               = 5,
	Stat_CrosshairSizeMax                    = 6,
	Stat_Impulse                             = 7,
	Stat_AimOnDelay                          = 8,
	Stat_AimOffDelay                         = 9,
	Stat_JumpZVelocity                       = 10,
	Stat_CurrentEnergyShield                 = 11,
	Stat_InteractionRevive                   = 12,
	Stat_RunSpeed                            = 13,
	Stat_SprintSpeed                         = 14,
	Stat_ZoomMoveSpeed                       = 15,
	Stat_RoundsPerMagazine                   = 16,
	Stat_ReloadRounds                        = 17,
	Stat_FireInterval                        = 18,
	Stat_MaxMp                               = 19,
	Stat_MaxEnergyShield                     = 20,
	Stat_ATK                                 = 21,
	Stat_ATKMin                              = 22,
	Stat_ATKMax                              = 23,
	Stat_DEF                                 = 24,
	Stat_CriticalChance                      = 25,
	Stat_CriticalATKIncrease                 = 26,
	Stat_ShellCount                          = 27,
	Stat_RoundsConsume                       = 28,
	Stat_AimMoveSpeed                        = 29,
	Stat_WeaknessDamageIncrease              = 30,
	Stat_SpreadZoomSize_Deprecated           = 31,
	Stat_CrosshairMoveSizeModifier           = 32,
	Stat_CrosshairFireSizeModifier           = 33,
	Stat_CrosshairZoomFireSizeModifier       = 34,
	Stat_CrosshairRecoverySpeed              = 35,
	Stat_CrosshairChangeSpeed                = 36,
	Stat_ClimbSpeed                          = 37,
	Stat_StressMaxRate_Deprecated            = 38,
	Stat_StressDamageRate                    = 39,
	Stat_StressDecreaseRate                  = 40,
	Stat_NonCombatEntryTime_Deprecated       = 41,
	Stat_StressDecreaseStartTime             = 42,
	Stat_WalkSpeed                           = 43,
	Stat_MoveDestructivePower                = 44,
	Stat_WeaponProjHitRadiusCoefficient      = 45,
	Stat_EvadeTime                           = 46,
	Stat_EvadeRecoveryTime                   = 47,
	Stat_Domination                          = 48,
	Stat_Manipulation                        = 49,
	Stat_Fortitude                           = 50,
	Stat_CrosshairSizeMin                    = 51,
	Stat_ReloadSpeed                         = 52,
	Stat_ReloadCompleteDelay                 = 53,
	Stat_ReduceRangeStart                    = 54,
	Stat_ReduceRangeEnd                      = 55,
	Stat_ATKReductionByRange                 = 56,
	Stat_ATKExplosion                        = 57,
	Stat_ATKBlazer                           = 58,
	Stat_ATKGlacier                          = 59,
	Stat_ATKElectricity                      = 60,
	Stat_ATKDemonic                          = 61,
	Stat_DEFBlazer                           = 62,
	Stat_DEFGlacier                          = 63,
	Stat_DEFElectricity                      = 64,
	Stat_DEFDemonic                          = 65,
	Stat_RESCriticalChance                   = 66,
	Stat_ReloadTime                          = 67,
	Stat_Burst                               = 68,
	Stat_Crush                               = 69,
	Stat_Piercing                            = 70,
	Stat_FireBurstInterval                   = 71,
	Stat_ATKReductionByExplosionRange        = 72,
	Stat_ZoomOutDelay                        = 73,
	Stat_Penetration                         = 74,
	Stat_PenetrationDefence                  = 75,
	Stat_WeaponDestructivePower              = 76,
	Stat_SkillDestructivePower               = 77,
	Stat_FireIncreaseSpread_Deprecated       = 78,
	Stat_LifeTime_Deprecated                 = 79,
	Stat_BonusLifeTime_Deprecated            = 80,
	Stat_RangeMin_Deprecated                 = 81,
	Stat_WireCooltimeCoefficient             = 82,
	Stat_WireChargingTimeCoefficient         = 83,
	Stat_KnockbackAttackRate                 = 84,
	Stat_KnockbackDefenceResistance          = 85,
	Stat_KnockbackTimeResistance             = 86,
	Stat_KnockbackSpeedResistance            = 87,
	Stat_KnockdownAttackRate                 = 88,
	Stat_KnockdownXSpeed                     = 89,
	Stat_KnockdownZSpeed                     = 90,
	Stat_KnockdownDefenceResistance          = 91,
	Stat_KnockdownXSpeedResistance           = 92,
	Stat_KnockdownZSpeedResistance           = 93,
	Stat_KnockdownTimeResistance             = 94,
	Stat_MeleeSpecialAttackReduceCoolTime_Deprecated = 95,
	Stat_HitStopAttackPoint                  = 96,
	Stat_HitStopDefencePoint                 = 97,
	Stat_BerserkMoveSpeed                    = 98,
	Stat_BerserkMaxRate                      = 99,
	Stat_BerserkDamageRate                   = 100,
	Stat_BerserkDecreaseStartTime            = 101,
	Stat_BerserkDecreaseRate                 = 102,
	Stat_BerserkMaintenanceTime              = 103,
	Stat_NaturalRecoveryTickTimeHp           = 104,
	Stat_NaturalRecoveryTickTimeMp           = 105,
	Stat_NaturalRecoveryTickTimeEnergyShield = 106,
	Stat_NaturalRecoveryHp                   = 107,
	Stat_NaturalRecoveryMp                   = 108,
	Stat_NaturalRecoveryEnergyShield         = 109,
	Stat_InBattleNaturalRecoveryHp           = 110,
	Stat_InBattleNaturalRecoveryMp           = 111,
	Stat_InBattleNaturalRecoveryEnergyShield = 112,
	Stat_CurrentHp                           = 113,
	Stat_CurrentMp                           = 114,
	Stat_MaxCommonGauge                      = 115,
	Stat_CurrentCommonGauge                  = 116,
	Stat_DEFInflection_VarX1_Deprecated      = 117,
	Stat_DEFInflection_VarX2_Deprecated      = 118,
	Stat_DEFInflection_VarX3_Deprecated      = 119,
	Stat_WireStackCountIncrease              = 120,
	Stat_RecoilCoefficient                   = 121,
	Stat_SkillCooltimeCoefficient            = 122,
	Stat_SkillCostCoefficient                = 123,
	Stat_SkillATKCoefficient                 = 124,
	Stat_FinalDamageCoefficient_Deprecated   = 125,
	Stat_HpOrbRecoveryCoefficient_Deprecated = 126,
	Stat_MpOrbRecoveryCoefficient_Deprecated = 127,
	Stat_EmptyRoundsReloadTimeSpeed          = 128,
	Stat_SkillDurationIncrease               = 129,
	Stat_SkillDurationDecrease               = 130,
	Stat_SkillScaleCoefficient               = 131,
	Stat_WeaponChangingSpeed                 = 132,
	Stat_MaxGeneralRoundsCoefficient         = 133,
	Stat_MaxEnhancedRoundsCoefficient        = 134,
	Stat_MaxImpactRoundsCoefficient          = 135,
	Stat_MaxHighpowerRoundsCoefficient       = 136,
	Stat_NaturalRecoveryTickPR_Deprecated    = 137,
	Stat_InBattleNaturalRecoveryPR_Deprecated = 138,
	Stat_NaturalRecoveryPR_Deprecated        = 139,
	Stat_CrosshairJumpSizeModifier           = 140,
	Stat_PhysicalATK                         = 141,
	Stat_NatureATK                           = 142,
	Stat_GiveHpRecoveryCoefficient           = 143,
	Stat_TakeHpRecoveryCoefficient           = 144,
	Stat_GiveMpRecoveryCoefficient           = 145,
	Stat_TakeMpRecoveryCoefficient           = 146,
	Stat_RWATKCoefficient                    = 147,
	Stat_GiveFinalDamageCoefficient          = 148,
	Stat_TakeFinalDamageCoefficient          = 149,
	Stat_LevelFactor                         = 150,
	Stat_SkillATKCorrection                  = 151,
	Stat_ItemLevelCorrection                 = 152,
	Stat_MaxStressGauge                      = 153,
	Stat_CurrentStressGauge                  = 154,
	Stat_MaxBerserkGauge                     = 155,
	Stat_CurrentBerserkGauge                 = 156,
	Stat_SkillATKPower                       = 157,
	Stat_SkillPowerCoefficient               = 158,
	Stat_SkillPlainPowerCoefficient          = 159,
	Stat_SkillBlazerPowerCoefficient         = 160,
	Stat_SkillGlacierPowerCoefficient        = 161,
	Stat_SkillElectricityPowerCoefficient    = 162,
	Stat_SkillDemonicPowerCoefficient        = 163,
	Stat_SkillFusionPowerCoefficient         = 164,
	Stat_SkillSingularPowerCoefficient       = 165,
	Stat_SkillDimensionPowerCoefficient      = 166,
	Stat_SkillTechPowerCoefficient           = 167,
	Stat_SkillPlainATKCoefficient            = 168,
	Stat_SkillBlazerATKCoefficient           = 169,
	Stat_SkillGlacierATKCoefficient          = 170,
	Stat_SkillElectricityATKCoefficient      = 171,
	Stat_SkillDemonicATKCoefficient          = 172,
	Stat_SkillFusionATKCoefficient           = 173,
	Stat_SkillSingularATKCoefficient         = 174,
	Stat_SkillDimensionATKCoefficient        = 175,
	Stat_SkillTechATKCoefficient             = 176,
	Stat_SkillCriticalChance                 = 178,
	Stat_SkillCriticalATKIncrease            = 179,
	Stat_SkillRESCriticalChance              = 180,
	Stat_DEFSkill                            = 181,
	Stat_DEFPlain                            = 182,
	Stat_Accuracy                            = 183,
	Stat_ZoomAccuracy                        = 184,
	Stat_InvokeChanceElementalSE             = 185,
	Stat_GiveShieldRecoveryCoefficient       = 186,
	Stat_TakeShieldRecoveryCoefficient       = 187,
	Stat_MaxSprintSpeed                      = 188,
	Stat_WeaponProjHitRadius                 = 189,
	Stat_DBNORescueTime                      = 190,
	Stat_RuneDropCoefficient                 = 191,
	Stat_EquipmentDropCoefficient            = 192,
	Stat_ConsumableDropCoefficient           = 193,
	Stat_GoldDropCoefficient                 = 194,
	Stat_QulipothiumDropCoefficient          = 195,
	Stat_CharEXPCoefficient                  = 196,
	Stat_ProficiencyCoefficient              = 197,
	Stat_TypeBossATKBonus                    = 198,
	Stat_TypeDarknessATKBonus                = 199,
	Stat_TypeTruthATKBonus                   = 200,
	Stat_TypeImmortalATKBonus                = 201,
	Stat_DBNODurationCoefficient             = 202,
	Stat_LootDistanceCoefficient             = 203,
	Stat_DoubleJumpZVelocity                 = 204,
	Stat_EciveSearchDistanceCoefficient      = 205,
	Stat_EciveSearchRemainTimeCoefficient    = 206,
	Stat_SubSkillATKPower                    = 207,
	Stat_SubSkillATKCoefficient              = 208,
	Stat_TakeWeaknessDamageCoefficient_Deprecated = 209,
	Stat_RWChargeRateCoefficient             = 210,
	Stat_RWDischargeRateCoefficient          = 211,
	Stat_TypeBossSkillATKBonus               = 212,
	Stat_TypeDarknessSkillATKBonus           = 213,
	Stat_TypeTruthSkillATKBonus              = 214,
	Stat_TypeImmortalSkillATKBonus           = 215,
	Stat_BurstDelay                          = 216,
	Stat_DefaultStressStopDuration           = 217,
	Stat_FieldTimingRing_LimitTimeRatio      = 218,
	Stat_FieldTimingRing_NeedleRotationTimeRatio = 219,
	Stat_FieldTimingRing_ReduceSuccessCount  = 220,
	Stat_FieldTimingRing_IncreaseTimingAngleMax = 221,
	Stat_FieldTimingRing_FailTimeRatio       = 222,
	Stat_SkillVelocityAndRangeCoefficient    = 223,
	Stat_StressMaxHpRandMinRate              = 224,
	Stat_StressMaxHpRandMaxRate              = 225,
	Stat_StressMaxHpCurrentRate              = 226,
	Stat_ActionSpeedCoeffcient               = 227,
	Stat_ProjectilePenetration               = 228,
	Stat_ProjectilePenetrationDefence        = 229,
	Stat_RageMaxRate                         = 230,
	Stat_RageDamageRate                      = 231,
	Stat_RageDecreaseRate                    = 232,
	Stat_MaxRageGauge                        = 233,
	Stat_CurrentRageGauge                    = 234,
	Stat_MultiHitChance                      = 235,
	Stat_AdvantageCoefficient                = 236,
	MAX                                      = 237,
};

// Enum M1Data.EM1BattlePassLevelReason
// NumValues: 0x0004
enum class EM1BattlePassLevelReason : uint32
{
	NormalExpUp                              = 0,
	LevelUpBuy                               = 1,
	PremiumExclusiveRepeal                   = 2,
	EM1BattlePassLevelReason_MAX             = 3,
};

// Enum M1Data.EM1BattlePassReason
// NumValues: 0x0010
enum class EM1BattlePassReason : uint32
{
	Fail                                     = 0,
	Success                                  = 1,
	InvalidSeason                            = 2,
	AlreadyGetPremium                        = 3,
	UnknownError                             = 4,
	InvalidData                              = 5,
	NotEnoughResource                        = 6,
	InvalidCurrency                          = 7,
	TryLevelUpOver                           = 8,
	AlreadyBuyBundle                         = 9,
	OldSeasonNotCompleted                    = 10,
	CantBuyLevelCauseOver                    = 11,
	AlreadyGetPremiumExlusiveReward          = 12,
	CantGetReward                            = 13,
	AdjustmentPeriod                         = 14,
	EM1BattlePassReason_MAX                  = 15,
};

// ScriptStruct M1Data.M1QuestData
// 0x00E8 (0x00F0 - 0x0008)
struct FM1QuestData : public FTableRowBase
{
public:
	struct FM1TemplateId                          TemplateId;                                        // 0x0008(0x0004)(Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FName                                   StringIdName;                                      // 0x000C(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	EM1QuestType                                  Type;                                              // 0x0014(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_15[0x3];                                       // 0x0015(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
	TArray<struct FM1QuestStartPoint>             StartPoints;                                       // 0x0018(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	TArray<struct FM1QuestStartCondition>         StartConditions;                                   // 0x0028(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	struct FM1QuestCompleteConditionGroupInfo     CompleteConditionGroupInfo;                        // 0x0038(0x000C)(Edit, NoDestructor, NativeAccessSpecifierPublic)
	uint8                                         Pad_44[0x4];                                       // 0x0044(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	TArray<struct FM1CompleteCondition>           CompleteConditions;                                // 0x0048(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	TArray<struct FM1QuestDirection>              QuestDirections;                                   // 0x0058(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	struct FM1OpenField                           OpenField;                                         // 0x0068(0x0020)(Edit, NativeAccessSpecifierPublic)
	int64                                         RewardCharacterExp;                                // 0x0088(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int64                                         RewardMasteryExp;                                  // 0x0090(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int64                                         RewardSeasonExp;                                   // 0x0098(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<struct FM1CurrencyPair>                RewardCurrencies;                                  // 0x00A0(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	TArray<struct FM1QuestRewardItem>             RewardItems;                                       // 0x00B0(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	TArray<struct FM1PlayerDataLink>              RequiredCharacterIds;                              // 0x00C0(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	class FString                                 OutgoingMailId;                                    // 0x00D0(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         SubTrackerPriority;                                // 0x00E0(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         ProgressId;                                        // 0x00E4(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FName                                   QuestMarkerTarget;                                 // 0x00E8(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// ScriptStruct M1Data.M1BoostExpData
// 0x0010 (0x0010 - 0x0000)
struct FM1BoostExpData final
{
public:
	EM1BoostType                                  Type;                                              // 0x0000(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1[0x7];                                        // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	int64                                         BoostExp;                                          // 0x0008(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// ScriptStruct M1Data.M1CharacterAddExpNoti
// 0x0038 (0x0038 - 0x0000)
struct FM1CharacterAddExpNoti final
{
public:
	int64                                         AccountUid;                                        // 0x0000(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FM1TemplateId                          PlayerTid;                                         // 0x0008(0x0004)(Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_C[0x4];                                        // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	int64                                         Exp;                                               // 0x0010(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int64                                         BoostExp;                                          // 0x0018(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	TArray<struct FM1BoostExpData>                BoostDataList;                                     // 0x0020(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	EM1ExpNotiType                                NotiType;                                          // 0x0030(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_34[0x4];                                       // 0x0034(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

// ScriptStruct M1Data.M1StatValuePair
// 0x0010 (0x0010 - 0x0000)
struct FM1StatValuePair final
{
public:
	EM1StatType                                   StatTypeEnum;                                      // 0x0000(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1[0x7];                                        // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	double                                        StatValue;                                         // 0x0008(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

/ ScriptStruct M1Data.M1BoostItemEffect
// 0x0030 (0x0030 - 0x0000)
struct FM1BoostItemEffect final
{
public:
	int64                                         AccountUid;                                        // 0x0000(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	EM1BuffSubItemType                            BuffType;                                          // 0x0008(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_9[0x7];                                        // 0x0009(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	int64                                         BoostGroupId;                                      // 0x0010(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                         BoostValue;                                        // 0x0018(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1C[0x4];                                       // 0x001C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	struct FDateTime                              EndTime;                                           // 0x0020(0x0008)(Edit, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FM1TemplateId                          newBoostItemTid;                                   // 0x0028(0x0004)(Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_2C[0x4];                                       // 0x002C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

// ScriptStruct M1Data.M1MasteryCanLevelUpNoti
// 0x0010 (0x0010 - 0x0000)
struct FM1MasteryCanLevelUpNoti final
{
public:
	int64                                         AccountUid;                                        // 0x0000(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FM1TemplateId                          Level;                                             // 0x0008(0x0004)(Edit, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_C[0x4];                                        // 0x000C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};

// ScriptStruct M1Data.M1CurrencyInternalTran
// 0x0010 (0x0010 - 0x0000)
struct FM1CurrencyInternalTran final
{
public:
	EM1CurrencyInternalType                       Type;                                              // 0x0000(0x0002)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_2[0x6];                                        // 0x0002(0x0006)(Fixing Size After Last Property [ Dumper-7 ])
	int64                                         Count;                                             // 0x0008(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// ScriptStruct M1Data.M1BoostExpData
// 0x0010 (0x0010 - 0x0000)
struct FM1BoostExpData final
{
public:
	EM1BoostType                                  Type;                                              // 0x0000(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                         Pad_1[0x7];                                        // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
	int64                                         BoostExp;                                          // 0x0008(0x0008)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};