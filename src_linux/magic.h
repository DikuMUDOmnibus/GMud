/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1998 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@hypercube.org)				   *
*	    Gabrielle Taylor (gtaylor@hypercube.org)			   *
*	    Brian Moore (zump@rom.org)					   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/


/*
 * Spell functions.
 * Defined in magic.c.
 */
DECLARE_SPELL_FUN(	spell_null		);
DECLARE_SPELL_FUN(	spell_acid_blast	);
DECLARE_SPELL_FUN(	spell_armor		);
DECLARE_SPELL_FUN(	spell_bless		);
DECLARE_SPELL_FUN(	spell_blindness		);
DECLARE_SPELL_FUN(	spell_burning_hands	);
DECLARE_SPELL_FUN(	spell_call_lightning	);
DECLARE_SPELL_FUN(      spell_calm		);
DECLARE_SPELL_FUN(      spell_cancellation	);
DECLARE_SPELL_FUN(	spell_cause_critical	);
DECLARE_SPELL_FUN(	spell_cause_light	);
DECLARE_SPELL_FUN(	spell_cause_serious	);
DECLARE_SPELL_FUN(	spell_change_sex	);
DECLARE_SPELL_FUN(      spell_chain_lightning   );
DECLARE_SPELL_FUN(	spell_charm_person	);
DECLARE_SPELL_FUN(	spell_chill_touch	);
DECLARE_SPELL_FUN(	spell_colour_spray	);
DECLARE_SPELL_FUN(	spell_continual_light	);
DECLARE_SPELL_FUN(	spell_control_weather	);
DECLARE_SPELL_FUN(	spell_create_food	);
DECLARE_SPELL_FUN(	spell_create_rose	);
DECLARE_SPELL_FUN(	spell_create_spring	);
DECLARE_SPELL_FUN(	spell_create_water	);
DECLARE_SPELL_FUN(	spell_cure_blindness	);
DECLARE_SPELL_FUN(	spell_cure_critical	);
DECLARE_SPELL_FUN(      spell_cure_disease	);
DECLARE_SPELL_FUN(	spell_cure_light	);
DECLARE_SPELL_FUN(	spell_cure_poison	);
DECLARE_SPELL_FUN(	spell_cure_serious	);
DECLARE_SPELL_FUN(	spell_curse		);
DECLARE_SPELL_FUN(      spell_demonfire		);
DECLARE_SPELL_FUN(	spell_detect_evil	);
DECLARE_SPELL_FUN(	spell_detect_good	);
DECLARE_SPELL_FUN(	spell_detect_hidden	);
DECLARE_SPELL_FUN(	spell_detect_invis	);
DECLARE_SPELL_FUN(	spell_detect_magic	);
DECLARE_SPELL_FUN(	spell_detect_poison	);
DECLARE_SPELL_FUN(	spell_dispel_evil	);
DECLARE_SPELL_FUN(      spell_dispel_good       );
DECLARE_SPELL_FUN(	spell_dispel_magic	);
DECLARE_SPELL_FUN(	spell_earthquake	);
DECLARE_SPELL_FUN(	spell_enchant_armor	);
DECLARE_SPELL_FUN(	spell_enchant_weapon	);
DECLARE_SPELL_FUN(	spell_energy_drain	);
DECLARE_SPELL_FUN(	spell_faerie_fire	);
DECLARE_SPELL_FUN(	spell_faerie_fog	);
DECLARE_SPELL_FUN(	spell_farsight		);
DECLARE_SPELL_FUN(	spell_fireball		);
DECLARE_SPELL_FUN(	spell_fireproof		);
DECLARE_SPELL_FUN(	spell_flamestrike	);
DECLARE_SPELL_FUN(	spell_floating_disc	);
DECLARE_SPELL_FUN(	spell_fly		);
DECLARE_SPELL_FUN(      spell_frenzy		);
DECLARE_SPELL_FUN(	spell_gate		);
DECLARE_SPELL_FUN(	spell_giant_strength	);
DECLARE_SPELL_FUN(	spell_harm		);
DECLARE_SPELL_FUN(      spell_haste		);
DECLARE_SPELL_FUN(	spell_heal		);
DECLARE_SPELL_FUN(	spell_heat_metal	);
DECLARE_SPELL_FUN(      spell_holy_word		);
DECLARE_SPELL_FUN(	spell_identify		);
DECLARE_SPELL_FUN(	spell_infravision	);
DECLARE_SPELL_FUN(	spell_invis		);
DECLARE_SPELL_FUN(	spell_know_alignment	);
DECLARE_SPELL_FUN(	spell_lightning_strike	);
DECLARE_SPELL_FUN(	spell_locate_object	);
DECLARE_SPELL_FUN(	spell_magic_missile	);
DECLARE_SPELL_FUN(      spell_mass_healing	);
DECLARE_SPELL_FUN(	spell_mass_invis	);
DECLARE_SPELL_FUN(	spell_nexus		);
DECLARE_SPELL_FUN(	spell_pass_door		);
DECLARE_SPELL_FUN(      spell_plague		);
DECLARE_SPELL_FUN(	spell_poison		);
DECLARE_SPELL_FUN(	spell_portal		);
DECLARE_SPELL_FUN(	spell_protection_evil	);
DECLARE_SPELL_FUN(	spell_protection_good	);
DECLARE_SPELL_FUN(	spell_ray_of_truth	);
DECLARE_SPELL_FUN(	spell_recharge		);
DECLARE_SPELL_FUN(	spell_refresh		);
DECLARE_SPELL_FUN(	spell_remove_curse	);
DECLARE_SPELL_FUN(	spell_sanctuary		);
DECLARE_SPELL_FUN(	spell_shocking_grasp	);
DECLARE_SPELL_FUN(	spell_shield		);
DECLARE_SPELL_FUN(	spell_sleep		);
DECLARE_SPELL_FUN(	spell_slow		);
DECLARE_SPELL_FUN(	spell_stone_skin	);
DECLARE_SPELL_FUN(	spell_summon		);
DECLARE_SPELL_FUN(	spell_teleport		);
DECLARE_SPELL_FUN(	spell_ventriloquate	);
DECLARE_SPELL_FUN(	spell_weaken		);
DECLARE_SPELL_FUN(	spell_word_of_recall	);
DECLARE_SPELL_FUN(	spell_acid_breath	);
DECLARE_SPELL_FUN(	spell_fire_breath	);
DECLARE_SPELL_FUN(	spell_frost_breath	);
DECLARE_SPELL_FUN(	spell_gas_breath	);
DECLARE_SPELL_FUN(	spell_lightning_breath	);
DECLARE_SPELL_FUN(	spell_general_purpose	);
DECLARE_SPELL_FUN(	spell_high_explosive	);
DECLARE_SPELL_FUN( spell_summon_ggolem  );

/* Spells added by Gremlin */
DECLARE_SPELL_FUN(  spell_confusion     );
DECLARE_SPELL_FUN(  spell_brain_bulge     );
DECLARE_SPELL_FUN(  spell_faith     );
DECLARE_SPELL_FUN(  spell_vengeance     );
DECLARE_SPELL_FUN(  spell_smog     );
DECLARE_SPELL_FUN(  spell_smoke_screen     );
DECLARE_SPELL_FUN(  spell_enlightenment     );
DECLARE_SPELL_FUN(  spell_vitality     );
DECLARE_SPELL_FUN(  spell_vapour     );
DECLARE_SPELL_FUN(  spell_camouflage     );
DECLARE_SPELL_FUN(  spell_downdraft     );
DECLARE_SPELL_FUN(  spell_sulfur     );
DECLARE_SPELL_FUN(  spell_vacuum     );
DECLARE_SPELL_FUN(  spell_sand_storm     );
DECLARE_SPELL_FUN(  spell_gale_blast     );
DECLARE_SPELL_FUN(  spell_hurricane     );
DECLARE_SPELL_FUN(  spell_petrify     );
DECLARE_SPELL_FUN(  spell_ill_begotten     );
DECLARE_SPELL_FUN(  spell_surcease     );
DECLARE_SPELL_FUN(  spell_abominable_strike     );
DECLARE_SPELL_FUN(  spell_sorrow     );
DECLARE_SPELL_FUN(  spell_malfeasance     );
DECLARE_SPELL_FUN(  spell_spontaneous_combustion     );
DECLARE_SPELL_FUN(  spell_camouflage     );
DECLARE_SPELL_FUN(  spell_awe     );
DECLARE_SPELL_FUN(  spell_catharsis     );
DECLARE_SPELL_FUN(  spell_ablution     );
DECLARE_SPELL_FUN(  spell_sanctified_cleansing     );
DECLARE_SPELL_FUN(  spell_virtuous_blast     );
DECLARE_SPELL_FUN(  spell_archangel     );
DECLARE_SPELL_FUN(  spell_celestial_light     );
DECLARE_SPELL_FUN(  spell_thunder     );
DECLARE_SPELL_FUN(  spell_earthbind     );
DECLARE_SPELL_FUN(  spell_kinetic_blast     );
DECLARE_SPELL_FUN(  spell_parasite     );
DECLARE_SPELL_FUN(  spell_meteor_shower     );
DECLARE_SPELL_FUN(  spell_avalanche     );
DECLARE_SPELL_FUN(  spell_sink_hole     );
DECLARE_SPELL_FUN(  spell_rock_slide     );
DECLARE_SPELL_FUN(  spell_backdraft     );
DECLARE_SPELL_FUN(  spell_flare     );
DECLARE_SPELL_FUN(  spell_plasma_blast     );
DECLARE_SPELL_FUN(  spell_pyre_strike     );
DECLARE_SPELL_FUN(  spell_fire_storm     );
DECLARE_SPELL_FUN(  spell_magma_blast     );
DECLARE_SPELL_FUN(  spell_immolation     );
DECLARE_SPELL_FUN(  spell_waterlog     );
DECLARE_SPELL_FUN(  spell_nitrogen_shower     );
DECLARE_SPELL_FUN(  spell_hail     );
DECLARE_SPELL_FUN(  spell_geyser     );
DECLARE_SPELL_FUN(  spell_flash_flood     );
DECLARE_SPELL_FUN(  spell_typhoon     );
DECLARE_SPELL_FUN(  spell_tsunami     );

/* spells added by TAKA */
DECLARE_SPELL_FUN(	spell_stone_meld	);
DECLARE_SPELL_FUN(	spell_screen	);
DECLARE_SPELL_FUN(	spell_adrenaline	);
DECLARE_SPELL_FUN(	spell_quench		);
DECLARE_SPELL_FUN(	spell_sate			);
DECLARE_SPELL_FUN(	spell_acidrain		);
DECLARE_SPELL_FUN(	spell_icerain		);
DECLARE_SPELL_FUN(	spell_firerain		);
DECLARE_SPELL_FUN(	spell_firestorm		);
DECLARE_SPELL_FUN(	spell_acidstorm		);
DECLARE_SPELL_FUN(	spell_icestorm		);
DECLARE_SPELL_FUN(	spell_mud_skin		);
DECLARE_SPELL_FUN(	spell_moss_skin		);
DECLARE_SPELL_FUN(	spell_bark_skin		);
DECLARE_SPELL_FUN(	spell_steel_skin	);
DECLARE_SPELL_FUN(	spell_emerald_skin	);
DECLARE_SPELL_FUN(	spell_ruby_skin		);
DECLARE_SPELL_FUN(	spell_diamond_skin	);
DECLARE_SPELL_FUN(	spell_hit				);
DECLARE_SPELL_FUN(	spell_damage			);
DECLARE_SPELL_FUN(	spell_mind_flail		);
DECLARE_SPELL_FUN(	spell_psychic_thrust		);
DECLARE_SPELL_FUN(	spell_psychic_crush		);
DECLARE_SPELL_FUN(	spell_ego_whip			);
DECLARE_SPELL_FUN(	spell_mystic_armor		);
DECLARE_SPELL_FUN(	spell_bloody_tears		);
DECLARE_SPELL_FUN(	spell_mind_bolt			);
DECLARE_SPELL_FUN(	spell_fighting_trance	);
DECLARE_SPELL_FUN(	spell_ice_bolt			);
DECLARE_SPELL_FUN(	spell_fire_bolt			);
DECLARE_SPELL_FUN(	spell_acid_bolt			);
DECLARE_SPELL_FUN(	spell_gas_bolt			);
DECLARE_SPELL_FUN(	spell_lightning_bolt		);
DECLARE_SPELL_FUN(	spell_holy_bolt			);
DECLARE_SPELL_FUN(	spell_fireblast 		);
DECLARE_SPELL_FUN(	spell_iceblast			);
DECLARE_SPELL_FUN(	spell_electricblast		);
DECLARE_SPELL_FUN(	spell_gasblast			);
DECLARE_SPELL_FUN(	spell_lightningblast	);
DECLARE_SPELL_FUN(	spell_holyblast			);
DECLARE_SPELL_FUN(	spell_more_xp			);
DECLARE_SPELL_FUN(	spell_acid_cloud		);
DECLARE_SPELL_FUN(	spell_fire_cloud		);
DECLARE_SPELL_FUN(	spell_frost_cloud		);
DECLARE_SPELL_FUN(	spell_gas_cloud			);
DECLARE_SPELL_FUN(	spell_lightning_cloud	);
DECLARE_SPELL_FUN(	spell_bite				);







/* end of magic.h new line follows here */





