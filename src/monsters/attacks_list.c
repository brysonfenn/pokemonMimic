#include "attacks.h"

#include <stdint.h>

#include "../player.h"
#include "conditions.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"

#include "attacks_special.h"
#include "pokemon.h"

#define NUM_ATTACKS 210

attack empty_attack=  {"-------------",  0,  0, 100, 100, NO_TYPE,  false, &attack_do_nothing };
//				 	   "Name         "  id  pp  pwr  acc    type     priority    effect         params
attack tackle       = {"Tackle"       ,  1, 35,  40,       95, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack scratch      = {"Scratch"      ,  2, 35,  40,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack growl        = {"Growl"        ,  3, 40,   0,      100, NORMAL,   false, &decrement_opponent_stat, ATTACK_STAT, 100 };
attack tail_whip    = {"Tail Whip"    ,  4, 30,   0,      100, NORMAL,   false, &decrement_opponent_stat, DEFENSE_STAT, 100 };
attack string_shot  = {"String Shot"  ,  5, 40,   0,       95, NORMAL,   false, &decrement_opponent_stat, SPEED_STAT, 100 };
attack poison_sting = {"Poison Sting" ,  6, 35,  25,      100, POISON,   false, &inflict_condition, POISONED, 30 };
attack sand_attack  = {"Sand Attack"  ,  7, 15,   0,      100, NORMAL,   false, &decrement_opponent_stat, ACCURACY_STAT, 100 };
attack quick_attack = {"Quick Attack" ,  8, 30,  40,      100, NORMAL,   true,  &attack_do_nothing, NO_CONDITION, 0 };
attack defense_curl = {"Defense Curl" ,  9, 40,   0,      100, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack vine_whip    = {"Vine whip"    , 10, 25,  45,      100, GRASS,    false, &attack_do_nothing, NO_CONDITION, 0 };

attack leech_seed   = {"Leech seed"   , 11, 10,   0,       90, GRASS,    false, &inflict_condition, SEEDED, 100 };
attack ember        = {"Ember"        , 12, 25,  40,      100, FIRE,     false, &inflict_condition, BURNED, 10 };
attack bubble       = {"Bubble"       , 13, 30,  20,      100, WATER,    false, &decrement_opponent_stat, SPEED_STAT, 10 };
attack poison_powder= {"Poison Powder", 14, 35,   0,       75, POISON,   false, &inflict_condition, POISONED, 100 };
attack sleep_powder = {"Sleep Powder" , 15, 15,   0,       75, GRASS,    false, &inflict_condition, ASLEEP, 100 };
attack razor_leaf   = {"Razor Leaf"   , 16, 25,  55,       95, GRASS,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack metal_claw   = {"Metal Claw"   , 17, 35,  50,       95, STEEL,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack smoke_screen = {"Smoke Screen" , 18, 20,   0,      100, NORMAL,   false, &decrement_opponent_stat, ACCURACY_STAT, 100 };
attack sweet_scent  = {"Sweet Scent"  , 19, 20,   0,  NO_MISS, GRASS,    false, &increment_self_stat, EVASIVENESS_STAT, 100 };
attack growth       = {"Growth"       , 20, 40,   0,  NO_MISS, GRASS,    false, &increment_self_stat, SP_ATTACK_STAT, 100 };

attack scary_face   = {"Scary Face"   , 21, 15,   0,       90, NORMAL,   false, &decrement_opponent_stat2, SPEED_STAT, 100 };
attack flame_thrower= {"Flame Thrower", 22, 15,  95,      100, FIRE,     false, &inflict_condition, BURNED, 10 };
attack slash        = {"Slash"        , 23, 20,  70,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack dragon_rage  = {"Dragon Rage"  , 24, 10,   0,      100, DRAGON,   false, &deal_specific_damage, NO_CONDITION, 40};
attack fire_spin    = {"Fire Spin"    , 25, 15,  15,       70, FIRE,     false, &inflict_condition, FIRE_SPINNED, 0 };
attack wing_attack  = {"Wing Attack"  , 26, 35,  60,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack withdraw     = {"Withdraw"     , 27, 40,   0,  NO_MISS, WATER,    false, &increment_self_stat, DEFENSE_STAT, 100 };
attack water_gun    = {"Water Gun"    , 28, 25,  40,      100, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack bite         = {"Bite"         , 29, 25,  60,      100, DARK,     false, &inflict_condition, FLINCHED, 30 };
attack rapid_spin   = {"Rapid Spin"   , 30, 40,  20,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };

attack protect      = {"Protect"      , 31, 10,   0,       75, NORMAL,   true,  &self_inflict_condition, PROTECTED, 100 };
attack skull_bash   = {"Skull Bash"   , 32, 15, 100,      100, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack hydro_pump   = {"Hydro Pump"   , 33,  5, 120,       80, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack harden       = {"Harden"       , 34, 30,   0,  NO_MISS, NORMAL,   false, &increment_self_stat, DEFENSE_STAT, 100 };
attack supersonic   = {"Supersonic"   , 35, 20,   0,       55, NORMAL,   false, &inflict_condition, CONFUSED, 100 };
attack confusion    = {"Confusion"    , 36, 25,  50,      100, PSYCHIC,  false, &inflict_condition, CONFUSED, 10 };
attack stun_spore   = {"Stun Spore"   , 37, 30,   0,       75, GRASS,    false, &inflict_condition, PARALYZED, 100 };
attack gust         = {"Gust"         , 38, 35,  40,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack psybeam      = {"Psybeam"      , 39, 20,  65,      100, PSYCHIC,  false, &inflict_condition, CONFUSED, 10 };
attack silver_wind  = {"Silver Wind"  , 40,  5,  60,      100, BUG,      false, &increment_self_stat, SP_ATTACK_STAT, 10 };

attack fury_attack  = {"Fury Attack"  , 41, 20,  15,       85, NORMAL,   false, &hit_2_5_times, NO_CONDITION, 0 };
attack pursuit      = {"Pursuit"      , 42, 10,  40,      100, DARK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack agility      = {"Agility"      , 43, 20,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat2, SPEED_STAT, 100 };
attack twineedle    = {"Twineedle"    , 44, 20,  25,      100, BUG,      false, &hit_twice, NO_CONDITION, 0 };
attack pin_missile  = {"Pin Missile"  , 45, 20,  14,       85, BUG,      false, &hit_2_5_times, NO_CONDITION, 0 };
attack hyper_fang   = {"Hyper Fang"   , 46, 15,  80,       90, NORMAL,   false, &inflict_condition, FLINCHED, 10 };
attack super_fang   = {"Super Fang"   , 47, 10,   0,       90, NORMAL,   false, &deal_percentage_damage, NO_CONDITION, 50 };
attack feather_dance= {"Feather Dance", 48, 15,   0,      100, FLYING,   false, &decrement_opponent_stat2, ATTACK_STAT, 100 };
attack swift        = {"Swift"        , 49, 20,  60,  NO_MISS, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack fury_swipes  = {"Fury Swipes"  , 50, 15,  18,       80, NORMAL,   false, &hit_2_5_times, NO_CONDITION, 0 };

attack peck         = {"Peck"         , 51, 35,  35,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack leer         = {"Leer"         , 52, 30,   0,      100, NORMAL,   false, &decrement_opponent_stat, DEFENSE_STAT, 100 };
attack aerial_ace   = {"Aerial Ace"   , 53, 20,  60,  NO_MISS, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack drill_peck   = {"Drill Peck"   , 54, 20,  80,      100, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack thunder_shock= {"Thunder Shock", 55, 30,  40,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 10 };
attack thunder_wave = {"Thunder Wave" , 56, 20,   0,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 100 };
attack double_team  = {"Double Team"  , 57, 15,   0,  NO_MISS, NORMAL,   false, &increment_self_stat, EVASIVENESS_STAT, 100 };
attack slam         = {"Slam"         , 58, 20,  80,       75, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack thunderbolt  = {"Thunderbolt"  , 59, 15,  95,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 10 };
attack thunder      = {"Thunder"      , 60, 10, 120,       70, ELECTRIC, false, &inflict_condition, PARALYZED, 30 };

attack double_kick  = {"Double Kick"  , 61, 30,  30,      100, FIGHTING, false, &hit_twice, NO_CONDITION, 0 };
attack crunch       = {"Crunch"       , 62, 15,  80,      100, DARK,     false, &decrement_opponent_stat, SP_DEFENSE_STAT, 20 };
attack body_slam    = {"Body Slam"    , 63, 15,  85,      100, NORMAL,   false, &inflict_condition, PARALYZED, 30 };
attack super_power  = {"Super Power"  , 64,  5, 120,      100, FIGHTING, false, &decrement_opponent_stat, ATTACK_STAT, 100 };
attack horn_attack  = {"Horn Attack"  , 65, 25,  65,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack horn_drill   = {"Horn Drill"   , 66,  5,   0,       30, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack thrash       = {"Thrash"       , 67, 20,  90,      100, NORMAL,   false, &thrash_move_func, ATTACK_SPECIAL_MOVE, NORMAL };
attack megahorn     = {"Megahorn"     , 68, 10, 120,       85, BUG,      false, &attack_do_nothing, NO_CONDITION, 0 };
attack pound        = {"Pound"        , 69, 35,  40,       95, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack sing         = {"Sing"         , 70, 15,   0,       55, NORMAL,   false, &inflict_condition, ASLEEP, 100 };

attack double_slap  = {"Double Slap"  , 71, 10,  15,       85, NORMAL,   false, &hit_2_5_times, NO_CONDITION, 0 };
attack minimize     = {"Minimize"     , 72, 20,   0,  NO_MISS, NORMAL,   false, &increment_self_stat, EVASIVENESS_STAT, 100 };
attack cosmic_power = {"Cosmic Power" , 73, 20,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat, SP_DEFENSE_STAT, 100 };
attack moonlight    = {"Moonlight"    , 74,  5,   0,  NO_MISS, NORMAL,   false, &self_heal, HP_PERCENTAGE, 50 };
attack meteor_mash  = {"Meteor Mash"  , 75, 10, 100,       85, STEEL,    false, &increment_self_stat, ATTACK_STAT, 100 };
attack rock_throw   = {"Rock Throw"   , 76, 15,  50,       90, ROCK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack synthesis    = {"Synthesis"    , 77,  5,   0,  NO_MISS, GRASS,    false, &self_heal, HP_PERCENTAGE, 50 };
attack solar_beam   = {"Solar Beam"   , 78, 10,  90,      100, GRASS,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack selfdestruct = {"Selfdestruct" , 79,  5, 400,      100, NORMAL,   false, &self_inflict_damage, HP_PERCENTAGE, 150 };
attack rock_blast   = {"Rock Blast"   , 80, 10,  25,       80, ROCK,     false, &hit_2_5_times, NO_CONDITION, 0 };

attack earthquake   = {"Earthquake"   , 81, 10, 100,      100, GROUND,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack explosion    = {"Explosion"    , 82,  5, 500,      100, NORMAL,   false, &self_inflict_damage, HP_PERCENTAGE, 150 };
attack double_edge  = {"Double-Edge"  , 83, 15, 120,      100, NORMAL,   false, &self_inflict_damage, PERCENT_DAMAGE_DEALT, 33 };
attack screech      = {"Screech"      , 84, 40,   0,       85, NORMAL,   false, &decrement_opponent_stat2, DEFENSE_STAT, 100 };
attack bind         = {"Bind"         , 85, 20,  15,       75, NORMAL,   false, &inflict_condition, TIGHT_HOLD, 100 };
attack iron_tail    = {"Iron Tail"    , 86, 35,  50,       95, STEEL,    false, &decrement_opponent_stat, DEFENSE_STAT, 30 };
attack sand_tomb    = {"Sand Tomb"    , 87, 20,  15,       75, GROUND,   false, &inflict_condition, SAND_TOMBED, 100 };
attack dragon_breath= {"Dragon Breath", 88, 20,  60,      100, DRAGON,   false, &inflict_condition, PARALYZED, 30 };
attack wrap         = {"Wrap"         , 89, 20,  15,       85, NORMAL,   false, &inflict_condition, TIGHT_HOLD, 100 };
attack glare        = {"Glare"        , 90, 30,   0,       75, NORMAL,   false, &inflict_condition, PARALYZED, 100 };

attack acid         = {"Acid"         , 91, 30,  40,      100, POISON,   false, &decrement_opponent_stat, DEFENSE_STAT, 10 };
attack will_o_wisp  = {"Will-O-Wisp"  , 92, 15,   0,       75, FIRE,     false, &inflict_condition, BURNED, 10 };
attack confuse_ray  = {"Confuse Ray"  , 93, 10,   0,      100, GHOST,    false, &inflict_condition, CONFUSED, 100 };
attack rest         = {"Rest"         , 94, 10,   0,  NO_MISS, NORMAL,   false, &rest_move_func, NO_CONDITION, 0 };
attack hyper_voice  = {"Hyper Voice"  , 95, 10,  90,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack leech_life   = {"Leech Life"   , 96, 15,  20,      100, BUG,      false, &self_heal, PERCENT_DAMAGE_DEALT, 50 };
attack astonish     = {"Astonish"     , 97, 15,  30,      100, GHOST,    false, &inflict_condition, FLINCHED, 30 };
attack air_cutter   = {"Air Cutter"   , 98, 25,  55,       95, FLYING,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack poison_fang  = {"Poison Fang"  , 99, 15,  50,      100, POISON,   false, &inflict_condition, POISONED, 30 };
attack absorb       = {"Absorb"       ,100, 20,  20,      100, GRASS,    false, &self_heal, PERCENT_DAMAGE_DEALT, 50 };

attack petal_dance  = {"Petal Dance"  ,101, 20,  90,      100, GRASS,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack mega_drain   = {"Mega Drain"   ,102, 20,  40,      100, GRASS,    false, &self_heal, PERCENT_DAMAGE_DEALT, 50 };
attack spore        = {"Spore"        ,103, 15,   0,      100, GRASS,    false, &inflict_condition, ASLEEP, 100 };
attack giga_drain   = {"Giga Drain"   ,104,  5,  60,      100, GRASS,    false, &self_heal, PERCENT_DAMAGE_DEALT, 50 };
attack psychic      = {"Psychic"      ,105, 10,  90,      100, PSYCHIC,  false, &decrement_opponent_stat, SP_DEFENSE_STAT, 10 };
attack magnitude    = {"Magnitude"    ,106, 30,  70,      100, GROUND,   false, &magnitude_move_func, ATTACK_SPECIAL_MOVE, 0 };
attack rollout      = {"Rollout"      ,107, 20,  70,      100, ROCK,     false, &rollout_move_func, ATTACK_SPECIAL_MOVE, 0 };
attack mud_slap     = {"Mud-Slap"     ,108, 10,  20,      100, GROUND,   false, &decrement_opponent_stat, ACCURACY_STAT, 100 };
attack swagger      = {"Swagger"      ,109, 15,   0,       90, NORMAL,   false, &swagger_move_func, NO_CONDITION, 0 };
attack fissure      = {"Fissure"      ,110,  5,   0,       30, GROUND,   false, &deal_specific_damage, NO_CONDITION, 5000 };

attack tri_attack   = {"Tri Attack"   ,111, 10,  80,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack pay_day      = {"Pay Day"      ,112, 20,  40,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack faint_attack = {"Faint Attack" ,113, 20,  60,  NO_MISS, DARK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack fake_out     = {"Fake Out"     ,114, 10,  40,      100, NORMAL,    true, &attack_do_nothing, NO_CONDITION, 0 };
attack disable      = {"Disable"      ,115, 20,   0,       90, NORMAL,   false, &inflict_condition, DISABLED, 100 };
attack low_kick     = {"Low Kick"     ,116, 20,  50,       90, FIGHTING, false, &inflict_condition, FLINCHED, 30 };
attack karate_chop  = {"Karate Chop"  ,117, 20,  70,      100, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack seismic_toss = {"Seismic Toss" ,118, 20,   0,      100, FIGHTING, false, &seismic_toss_move_func, NO_CONDITION, 0 };
attack cross_chop   = {"Cross Chop"   ,119,  5, 125,       80, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack take_down    = {"Take Down"    ,120, 20,  90,       85, NORMAL,   false, &self_inflict_damage, PERCENT_DAMAGE_DEALT, 25 };

attack flame_wheel  = {"Flame Wheel"  ,121, 25,  60,      100, FIRE,     false, &inflict_condition, BURNED, 10 };
attack extreme_speed= {"Extreme Speed",122,  5,  80,      100, NORMAL,    true, &attack_do_nothing, NO_CONDITION, 0 };
attack recover      = {"Recover"      ,123,  5,   0,  NO_MISS, NORMAL,   false, &self_heal, HP_PERCENTAGE, 50 };
attack bubble_beam  = {"Bubble Beam"  ,124, 20,  65,      100, WATER,    false, &decrement_opponent_stat, SPEED_STAT, 10 };
attack hypnosis     = {"Hypnosis"     ,125, 20,   0,       60, PSYCHIC,  false, &inflict_condition, ASLEEP, 100 };
attack submission   = {"Submission"   ,126, 25,  80,       80, FIGHTING, false, &self_inflict_damage, PERCENT_DAMAGE_DEALT, 25 };
attack stomp        = {"Stomp"        ,127, 20,  65,      100, NORMAL,   false, &inflict_condition, FLINCHED, 30 };
attack fire_blast   = {"Fire Blast"   ,128,  5, 120,       85, FIRE,     false, &inflict_condition, BURNED, 30 };
attack teleport     = {"Teleport"     ,129, 20,   0,  NO_MISS, PSYCHIC,  false, &teleport_move_func, NO_CONDITION, 100 };
attack kinesis      = {"Kinesis"      ,130, 15,   0,       80, PSYCHIC,  false, &decrement_opponent_stat, ACCURACY_STAT, 100 };

attack vital_throw  = {"Vital Throw"  ,131, 10,  70,  NO_MISS, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack dynamic_punch= {"Dynamic Punch",132,  5, 100,       50, FIGHTING, false, &inflict_condition, CONFUSED, 100 };
attack constrict    = {"Constrict"    ,133, 35,  10,      100, NORMAL,   false, &decrement_opponent_stat, SPEED_STAT, 10 };
attack barrier      = {"Barrier"      ,134, 30,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat2, DEFENSE_STAT, 100 };
attack amnesia      = {"Amnesia"      ,135, 20,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat2, SP_DEFENSE_STAT, 100 };
attack yawn         = {"Yawn"         ,136, 10,   0,  NO_MISS, PSYCHIC,  false, &inflict_condition, YAWNED, 100 };
attack headbutt     = {"Headbutt"     ,137, 15,  70,      100, NORMAL,   false, &inflict_condition, FLINCHED, 30 };
attack metal_sound  = {"Metal Sound"  ,138, 40,   0,       85, STEEL,    false, &decrement_opponent_stat, SP_DEFENSE_STAT, 100 };
attack sonic_boom   = {"Sonic Boom"   ,139, 15,  70,      100, NORMAL,   false, &deal_specific_damage, NO_CONDITION, 20 };
attack spark        = {"Spark"        ,140, 20,  65,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 30 };

attack zap_cannon   = {"Zap Cannon"   ,141,  5, 100,       50, ELECTRIC, false, &inflict_condition, PARALYZED, 100 };
attack knock_off    = {"Knock Off"    ,142, 20,  20,      100, DARK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack fury_cutter  = {"Fury Cutter"  ,143, 20,  50,       95, BUG,      false, &attack_do_nothing, NO_CONDITION, 0 };
attack swords_dance = {"Swords Dance" ,144, 20,   0,  NO_MISS, NORMAL,   false, &increment_self_stat2, ATTACK_STAT, 100 };
attack false_swipe  = {"False Swipe"  ,145, 40,  40,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack uproar       = {"Uproar"       ,146, 10,  50,      100, NORMAL,   false, &uproar_move_func, ATTACK_SPECIAL_MOVE, 0 };
attack icy_wind     = {"Icy Wind"     ,147, 15,  55,       95, ICE,      false, &decrement_opponent_stat, SPEED_STAT, 100 };
attack aurora_beam  = {"Aurora Beam"  ,148, 20,  65,      100, ICE,      false, &decrement_opponent_stat, ATTACK_STAT, 100 };
attack ice_beam     = {"Ice Beam"     ,149, 10,  95,      100, ICE,      false, &inflict_condition, FROZEN, 10 };
attack signal_beam  = {"Signal Beam"  ,150, 15,  75,      100, BUG,      false, &inflict_condition, CONFUSED, 10 };

attack sheer_cold   = {"Sheer Cold"   ,151,  5,   0,  NO_MISS, ICE,      false, &k_o_move_func, NO_CONDITION, 0 };
attack poison_gas   = {"Poison Gas"   ,152, 40,   0,       55, POISON,   false, &inflict_condition, POISONED, 100 };
attack sludge       = {"Sludge"       ,153, 20,  65,      100, POISON,   false, &inflict_condition, POISONED, 40 };
attack acid_armor   = {"Acid Armor"   ,154, 40,   0,  NO_MISS, POISON,   false, &increment_self_stat2, DEFENSE_STAT, 100 };
attack sludge_bomb  = {"Sludge Bomb"  ,155, 10,  90,      100, POISON,   false, &inflict_condition, POISONED, 30 };
attack icicle_spear = {"Icicle Spear" ,156, 30,  10,      100, ICE,      false, &hit_2_5_times, NO_CONDITION, 0 };
attack clamp        = {"Clamp"        ,157, 10,  35,       75, WATER,    false, &inflict_condition, TIGHT_HOLD, 100 };
attack spike_cannon = {"Spike Cannon" ,158, 15,  20,      100, NORMAL,   false, &hit_2_5_times, NO_CONDITION, 0 };
attack lick         = {"Lick"         ,159, 30,  20,      100, GHOST,    false, &inflict_condition, PARALYZED, 30 };
attack curse        = {"Curse"        ,160, 10,   0,  NO_MISS, GHOST,    false, &curse_move_func, ATTACK_SPECIAL_MOVE, 0 };

attack night_shade  = {"Night Shade"  ,161, 15,   0,      100, GHOST,    false, &night_shade_move_func, ATTACK_SPECIAL_MOVE, 0 };
attack shadow_ball  = {"Shadow Ball"  ,162, 15,  80,      100, GHOST,    false, &decrement_opponent_stat, SP_DEFENSE_STAT, 20 };
attack shadow_punch = {"Shadow Punch" ,163, 20,  60,  NO_MISS, GHOST,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack meditate     = {"Meditate"     ,164, 40,   0,  NO_MISS, PSYCHIC,  false, &increment_self_stat, ATTACK_STAT, 100 };
attack vice_grip    = {"Vice Grip"    ,165, 30,  55,      100, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack mud_shot     = {"Mud Shot"     ,166, 15,  55,       95, GROUND,   false, &decrement_opponent_stat, SPEED_STAT, 100 };
attack guillotine   = {"Guillotine"   ,167,  5,   0,  NO_MISS, NORMAL,   false, &k_o_move_func, NO_CONDITION, 0 };
attack crab_hammer  = {"Crab Hammer"  ,168, 10,  90,       85, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack barrage      = {"Barrage"      ,169, 20,  15,       85, NORMAL,   false, &hit_2_5_times, NO_CONDITION, 0 };
attack egg_bomb     = {"Egg Bomb"     ,170, 10, 100,       75, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };

attack bone_club    = {"Bone Club"    ,171, 20,  65,       85, GROUND,   false, &inflict_condition, FLINCHED, 10 };
attack bonemerang   = {"Bonemerang"   ,172, 10,  50,       90, GROUND,   false, &hit_twice, NO_CONDITION, 0 };
attack bone_rush    = {"Bone Rush"    ,173, 10,  25,       80, GROUND,   false, &hit_2_5_times, NO_CONDITION, 0 };
attack rolling_kick = {"Rolling Kick" ,174, 15,  60,       85, FIGHTING, false, &inflict_condition, FLINCHED, 30 };
attack jump_kick    = {"Jump Kick"    ,175, 25,  70,       95, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack brick_break  = {"Brick Break"  ,176, 15,  75,      100, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack hi_jump_kick = {"Hi Jump Kick" ,177, 20,  85,       90, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack mega_kick    = {"Mega Kick"    ,178,  5, 120,       75, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack comet_punch  = {"Comet Punch"  ,179, 15,  18,       85, NORMAL,   false, &hit_2_5_times, NO_CONDITION, 0 };
attack mach_punch   = {"Mach Punch"   ,180, 30,  40,      100, NORMAL,   true,  &attack_do_nothing, NO_CONDITION, 0 };

attack thunder_punch= {"Thunder Punch",181, 15,  75,      100, ELECTRIC, false, &inflict_condition, PARALYZED, 10 };
attack ice_punch    = {"Ice Punch"    ,182, 15,  75,      100, ICE,      false, &inflict_condition, FROZEN, 10 };
attack fire_punch   = {"Fire Punch"   ,183, 15,  75,      100, FIRE,     false, &inflict_condition, BURNED, 10 };
attack sky_uppercut = {"Sky Uppercut" ,184, 15,  85,       90, FIGHTING, false, &attack_do_nothing, NO_CONDITION, 0 };
attack mega_punch   = {"Mega Punch"   ,185, 20,  80,       85, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack detect       = {"Detect"       ,186,  5,   0,       75, FIGHTING,  true, &self_inflict_condition, PROTECTED, 100 };
attack refresh_move = {"Refresh"      ,187, 20,   0,  NO_MISS, NORMAL,   false, &refresh_move_func, NO_CONDITION, 0 };
attack smog         = {"Smog"         ,188, 20,  20,       70, POISON,   false, &inflict_condition, POISONED, 40 };
attack softboiled   = {"Softboiled"   ,189, 10,   0,  NO_MISS, NORMAL,   false, &self_heal, HP_PERCENTAGE, 50 };
attack ingrain      = {"Ingrain"      ,190, 20,   0,  NO_MISS, GRASS,    false, &self_inflict_condition, INGRAINED, 100 };

attack dizzy_punch  = {"Dizzy Punch"  ,191, 10,  70,      100, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };
attack twister      = {"Twister"      ,192, 20,  40,      100, DRAGON,   false, &inflict_condition, FLINCHED, 20 };
attack dragon_dance = {"Dragon Dance" ,193, 20,   0,  NO_MISS, DRAGON,   false, &increment_self_stat, ATTACK_STAT, 20 };
attack flail        = {"Flail"        ,194, 15,  40,      100, NORMAL,   false, &flail_move_func, ATTACK_SPECIAL_MOVE, 0 };
attack waterfall    = {"Waterfall"    ,195, 15,  80,      100, WATER,    false, &attack_do_nothing, NO_CONDITION, 0 };
attack magical_leaf = {"Magical Leaf" ,196, 20,  60,  NO_MISS, GRASS,    true,  &attack_do_nothing, NO_CONDITION, 0 };
attack lovely_kiss  = {"Lovely Kiss"  ,197, 10,   0,       75, NORMAL,   false, &inflict_condition, ASLEEP, 100 };
attack powder_snow  = {"Powder Snow"  ,198, 25,  40,      100, ICE,      false, &inflict_condition, FROZEN, 10 };
attack fake_tears   = {"Fake Tears"   ,199, 20,   0,      100, DARK,     false, &decrement_opponent_stat2, SP_DEFENSE_STAT, 100 };
attack blizzard     = {"Blizzard"     ,200,  5, 120,       70, ICE,      false, &inflict_condition, FROZEN, 30 };

attack splash       = {"Splash"       ,201, 40,   0,  NO_MISS, NORMAL,   false, &splash_move_func, NO_CONDITION, 0 };
attack sharpen      = {"Sharpen"      ,202, 30,   0,  NO_MISS, DRAGON,   false, &increment_self_stat, ATTACK_STAT, 20 };
attack ancient_power= {"Ancient Power",203,  5,  60,      100, ROCK,     false, &attack_do_nothing, NO_CONDITION, 0 };
attack heat_wave    = {"Heat Wave"    ,204, 10, 100,       90, FIRE,     false, &inflict_condition, BURNED, 10 };
attack outrage      = {"Outrage"      ,205, 15,  90,      100, NORMAL,   false, &thrash_move_func, ATTACK_SPECIAL_MOVE, DRAGON };
attack cut          = {"Cut"          ,206, 30,  50,       95, NORMAL,   false, &attack_do_nothing, NO_CONDITION, 0 };


static attack * local_array[NUM_ATTACKS] = { &empty_attack, 
    &tackle, &scratch, &growl, &tail_whip, &string_shot, &poison_sting, &sand_attack, &quick_attack, &defense_curl, &vine_whip,             // #01-10
    &leech_seed, &ember, &bubble, &poison_powder, &sleep_powder, &razor_leaf, &metal_claw, &smoke_screen, &sweet_scent, &growth,            // #11-20
    &scary_face, &flame_thrower, &slash, &dragon_rage, &fire_spin, &wing_attack, &withdraw, &water_gun, &bite, &rapid_spin,                 // #21-30
    &protect, &skull_bash, &hydro_pump, &harden, &supersonic, &confusion, &stun_spore, &gust, &psybeam, &silver_wind,                       // #31-40
    &fury_attack, &pursuit, &agility, &twineedle, &pin_missile, &hyper_fang, &super_fang, &feather_dance, &swift, &fury_swipes,             // #41-50
    &peck, &leer, &aerial_ace, &drill_peck, &thunder_shock, &thunder_wave, &double_team, &slam, &thunderbolt, &thunder,                     // #51-60
    &double_kick, &crunch, &body_slam, &super_power, &horn_attack, &horn_drill, &thrash, &megahorn, &pound, &sing,                          // #61-70
    &double_slap, &minimize, &cosmic_power, &moonlight, &meteor_mash, &rock_throw, &synthesis, &solar_beam, &selfdestruct, &rock_blast,     // #71-80
    &earthquake, &explosion, &double_edge, &screech, &bind, &iron_tail, &sand_tomb, &dragon_breath, &wrap, &glare,                          // #81-90
    &acid, &will_o_wisp, &confuse_ray, &rest, &hyper_voice, &leech_life, &astonish, &air_cutter, &poison_fang, &absorb,                     // #91-100
    &petal_dance, &mega_drain, &spore, &giga_drain, &psychic, &magnitude, &rollout, &mud_slap, &swagger, &fissure,                          // #101-110
    &tri_attack, &pay_day, &faint_attack, &fake_out, &disable, &low_kick, &karate_chop, &seismic_toss, &cross_chop, &take_down,             // #111-120
    &flame_wheel, &extreme_speed, &recover, &bubble_beam, &hypnosis, &submission, &stomp, &fire_blast, &teleport, &kinesis,                 // #121-130
    &vital_throw, &dynamic_punch, &constrict, &barrier, &amnesia, &yawn, &headbutt, &metal_sound, &sonic_boom, &spark,                      // #131-140
    &zap_cannon, &knock_off, &fury_cutter, &swords_dance, &false_swipe, &uproar, &icy_wind, &aurora_beam, &ice_beam, &signal_beam,          // #141-150
    &sheer_cold, &poison_gas, &sludge, &acid_armor, &sludge_bomb, &icicle_spear, &clamp, &spike_cannon, &lick, &curse,                      // #151-160
    &night_shade, &shadow_ball, &shadow_punch, &meditate, &vice_grip, &mud_shot, &guillotine, &crab_hammer, &barrage, &egg_bomb,            // #161-170
    &bone_club, &bonemerang, &bone_rush, &rolling_kick, &jump_kick, &brick_break, &hi_jump_kick, &mega_kick, &comet_punch, &mach_punch,     // #171-180
    &thunder_punch, &ice_punch, &fire_punch, &sky_uppercut, &mega_punch, &detect, &refresh_move, &smog, &softboiled, &ingrain,              // #181-190
    &dizzy_punch, &twister, &dragon_dance, &flail, &waterfall, &magical_leaf, &lovely_kiss, &powder_snow, &fake_tears, &blizzard,           // #191-200
    &splash, &sharpen, &ancient_power, &heat_wave, &outrage, &cut
};


//Return an attack given an attack id number
attack * get_attack_by_id(id_num) {
	return local_array[id_num];
}

//Filler side-effect function that does nothing
int attack_do_nothing(Condition condition, int accuracy, struct Pokemon* pok, int damage) {
	return ATTACK_SUCCESS;
}

//Change the stat of a pok (stat_type is the type of stat, stage_number is the number of stages to increment (+) or decrement (-))
int change_stat(Condition stat_type, int stage_number, struct Pokemon* pok) {
    char stat_type_str[50];
    int16_t * stat_stage;

    switch (stat_type) {
        case ATTACK_STAT:
            sprintf(stat_type_str, "%s", "attack");
            stat_stage = &(pok->atk_stage);
            break;
        case DEFENSE_STAT:
            sprintf(stat_type_str, "%s", "defense");
            stat_stage = &(pok->def_stage);
            break;
        case SP_ATTACK_STAT:
            sprintf(stat_type_str, "%s", "sp. attack");
            stat_stage = &(pok->sp_atk_stage);
            break;
        case SP_DEFENSE_STAT:
            sprintf(stat_type_str, "%s", "sp. defense");
            stat_stage = &(pok->sp_def_stage);
            break;
        case SPEED_STAT:
            sprintf(stat_type_str, "%s", "speed");
            stat_stage = &(pok->spd_stage);
            break;
        case ACCURACY_STAT:
            sprintf(stat_type_str, "%s", "accuracy");
            stat_stage = &(pok->acc_stage);
            break;
        case EVASIVENESS_STAT:
            sprintf(stat_type_str, "%s", "evasiveness");
            stat_stage = &(pok->evade_stage);
            break;
        default:
            sprintf(stat_type_str, "%s", "{unknown stat - default attack stat}");
            stat_stage = &(pok->atk_stage);
            break;
    }

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    
    //Drop Stat unless we are already 6 stages below
    if (stage_number < 0) {
        if (pok == player.enemy_pokemon) printw(ENEMY_TEXT);
        if ((*stat_stage) <= -6) {
            printw("%s's %s won't go any lower!", pok->name, stat_type_str);
        }
        else {
            printw("%s's %s ", pok->name, stat_type_str);
            if (stage_number == -1) printw("fell");
            else printw("harshly fell");
            (*stat_stage) += stage_number;

            if (*stat_stage < -6) (*stat_stage) = -6; //Stat stage can be -6 min
        }
        refresh(); sleep(2);
    }
    else if (stage_number > 0) {
        if (pok == player.enemy_pokemon) printw(ENEMY_TEXT);
        if ((*stat_stage) >= 6) {
            printw("%s's %s won't go any higher!", pok->name, stat_type_str);
        }
        else {
            printw("%s's %s ", pok->name, stat_type_str);
            if (stage_number == 1) printw("rose");
            else printw("sharply rose");
            (*stat_stage) += stage_number;

            if (*stat_stage > 6) (*stat_stage) = 6; //Stat stage can be 6 max
        }
        refresh(); sleep(2);
    }
}

//Certain attacks can increment a pokemon's own stat
int increment_self_stat(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, 1, self);
        return ATTACK_SUCCESS;
    }
    else return ATTACK_FAIL;
}


//Certain attacks can increment a pokemon's own stat
int increment_self_stat2(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, 2, self);
        return ATTACK_SUCCESS;
    }
    else return ATTACK_FAIL;
}

//Certain attacks can decrement an enemy pokemon's stat
int decrement_opponent_stat(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -1, victim);
        return ATTACK_SUCCESS;
    }
    else return ATTACK_FAIL;
}

int decrement_opponent_stat2(Condition stat_type, int chance, struct Pokemon* victim, int damage) {
    int random = rand() % 100;
    if (random < chance) {
        change_stat(stat_type, -2, victim);
        return ATTACK_SUCCESS;
    }
    else return ATTACK_FAIL;
}

int deal_specific_damage(Condition nothing1, int hp, struct Pokemon* victim, int nothing2) {
    bool enemy = (victim != player.enemy_pokemon);
    blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
    victim->currentHP -= hp;
    return ATTACK_SUCCESS;
}


//Some attacks hit multiple (2-5) times
int hit_2_5_times(int nothing1, int nothing2, struct Pokemon* victim, int damage) {

    bool enemy = (victim != player.enemy_pokemon);
    int rand_times;
    int i = 0;
    int rand_num = rand() % 100;

    //Random distribution
    if (rand_num < 35) rand_times = 2;
    else if (rand_num < 70) rand_times = 3;
    else if (rand_num < 85) rand_times = 4;
    else rand_times = 5;

    // Do not hit again if victim has fainted, hit up to random times - 1 (because we already hit once)
    while (victim->currentHP > 0 && i < rand_times-1) {
        sleep(1);
        blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
        victim->currentHP -= damage;
        if (victim->currentHP < 0) victim->currentHP = 0;
        i++;
        printBattle();
    }

    sleep(1);
    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("Hit %d time(s)", i+1); refresh(); sleep(2);

    return ATTACK_SUCCESS;
}


//Some attacks hit twice
int hit_twice(int nothing1, int nothing2, struct Pokemon* victim, int damage) {
    bool enemy = (victim != player.enemy_pokemon);
    int times = 1;

    // Do not hit again if victim has fainted
    if (victim->currentHP > 0) {
        sleep(1);
        blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
        victim->currentHP -= damage;
        times++;
        if (victim->currentHP < 0) victim->currentHP = 0;
        printBattle();
    }

    sleep(1);
    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("Hit %d time(s)", times); refresh(); sleep(2);
    return ATTACK_SUCCESS;
}


//Some attacks deal a percentage of damage left
int deal_percentage_damage(Condition nothing, int percent, struct Pokemon* victim, int damage) {
    bool enemy = (victim != player.enemy_pokemon);
    blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);

    float percent_f = ((float) percent) / 100.0;

    int hp = victim->currentHP * percent_f;
    if (hp < 1) hp = 1;
    victim->currentHP -= hp;
    
    return ATTACK_SUCCESS;
}


//Heal a specified percentage (1) or amount (2) of HP
int self_heal(int heal_type, int hp, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    //Do not heal if there is nothing to heal
    if (self->currentHP >= self->maxHP) {
        text_box_cursors(TEXT_BOX_BEGINNING);
        printw("%s is already at maximum HP!", self->name); refresh(); sleep(2);
        return ATTACK_FAIL;
    }

    float percentage;
    int gain;

    blinkPokemon(self == player.current_pokemon, HEAL_COLOR, 3);

    switch (heal_type) {
        case HP_PERCENTAGE:
            percentage = (hp / 100.0);
            self->currentHP += (int) (self->maxHP * percentage);
            break;
        case HP_AMOUNT:
            self->currentHP += hp;
            break;
        case PERCENT_DAMAGE_DEALT:
            gain = (int) (damage * (hp / 100.0));
            if (gain <= 0) gain = 1;
            self->currentHP += gain;
            break;
        default:
        text_box_cursors(TEXT_BOX_BEGINNING);
            printw("Unrecognized heal_type"); refresh(); sleep(2);
            return ATTACK_FAIL;
    }

    if (self->currentHP > self->maxHP) {
        self->currentHP = self->maxHP;
    }
    
    printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s restored HP!", self->name); refresh(); sleep(1);

    return ATTACK_SUCCESS;
}


//Inflict damage to self: damage_type = {HP_PERCENTAGE, HP_AMOUNT, PERCENT_DAMAGE_DEALT}
int self_inflict_damage(int damage_type, int hp, struct Pokemon* victim, int damage) {
    //Adjust victim depending on if the victim is the player's or the enemy's
    Pokemon * self;
    if (player.current_pokemon == victim) self = player.enemy_pokemon;
    else self = player.current_pokemon;

    float percentage;
    int recoil;

    blinkPokemon(self == player.current_pokemon, DAMAGED_COLOR, 3);

    switch (damage_type) {
        case HP_PERCENTAGE:
            percentage = (hp / 100.0);
            self->currentHP -= (int) (self->maxHP * percentage);
            break;
        case HP_AMOUNT:
            self->currentHP -= hp;
            break;
        case PERCENT_DAMAGE_DEALT:
            recoil = damage * (hp / 100.0);
            if (recoil <= 0) recoil = 1;
            self->currentHP -= recoil;
            break;
        default:
        text_box_cursors(TEXT_BOX_BEGINNING);
            printw("Unrecognized damage_type"); refresh(); sleep(2);
            return ATTACK_FAIL;
    }

    if (self->currentHP < 0) {
        self->currentHP = 0;
    }
    
    printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    printw("%s took damage too!", self->name); refresh(); sleep(1);

    return ATTACK_SUCCESS;
}