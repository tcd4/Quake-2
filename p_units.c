#include "g_local.h"
#include "m_player.h"

void unit_NextUnit (edict_t *ent)
{
	while (1)
	{
		ent->currentUnit++;
		if (ent->currentUnit >= MAX_UNITS)
		{
			ent->currentUnit = 0;
		}

		if (!ent->units[ ent->currentUnit ])
		{
			return;
		}

		if (ent->units[ ent->currentUnit ]->deadflag)
		{
			continue;
		}

		break;
	}
}

void unit_RunFrames (edict_t *ent, int start, int end)
{
	if (ent->s.frame >= start && ent->s.frame <= end)
	{
		ent->s.frame++;
	}
	else
	{
		ent->s.frame = start;
	}
}

qboolean unit_stand (edict_t *ent)
{
	if (VectorCompare (ent->s.old_origin, ent->s.origin))
	{
		return true;
	}

	return false;
}

void unit_die (edict_t *ent, edict_t *owner)
{
	ent->takedamage = DAMAGE_NO;
	ent->movetype = MOVETYPE_NONE;
	ent->svflags |= SVF_DEADMONSTER;
	ent->deadflag = DEAD_DEAD;
	ent->solid = SOLID_NOT;
	ent->svflags |= SVF_NOCLIENT;

	gi.sound (owner, CHAN_VOICE, gi.soundindex(va("*death%i.wav", (rand()%4)+1)), 1, ATTN_NORM, 0);
	
	gi.linkentity (ent);

	owner->client->resp.score--;
	if (owner->currentUnit == ent->thisUnit)
	{
		unit_NextUnit (owner);
	}
}

void unit_damage (edict_t *ent, edict_t *attacker, int damage)
{
	if (attacker == ent->owner)
	{
		return;
	}

	ent->health -= damage;

	if (ent->health <= 0)
	{
		unit_die (ent, ent->owner);
		return;
	}

	gi.sound (ent->owner, CHAN_VOICE, gi.soundindex(va("*pain%i_%i.wav", 50, 1 + (rand()&1))), 1, ATTN_NORM, 0);
}

void unit_move (edict_t *ent)
{
	VectorCopy (ent->owner->s.origin, ent->s.origin);
	VectorCopy (ent->owner->s.angles, ent->s.angles);

	gi.linkentity (ent);
}

void unit_think (edict_t *ent)
{
	if (unit_stand (ent))
	{
		unit_RunFrames (ent, FRAME_stand01, FRAME_stand40);
	}
	else
	{
		unit_RunFrames (ent, FRAME_run1, FRAME_run6);
	}

	ent->nextthink = level.time + .01;
}

void initUnit (edict_t *ent, int i, vec3_t	spawn_origin, vec3_t spawn_angles)
{
	vec3_t	mins = {-16, -16, -24};
	vec3_t	maxs = {16, 16, 32};

	ent->units[ i ] = G_Spawn ();
	ent->units[ i ]->classname = "playerunit";
	ent->units[ i ]->owner = ent;
	ent->units[ i ]->health = 100;
	ent->units[ i ]->takedamage = DAMAGE_AIM;
	ent->units[ i ]->movetype = MOVETYPE_STEP;
	ent->units[ i ]->viewheight = 22;
	ent->units[ i ]->inuse = true;
	ent->units[ i ]->mass = 200;
	ent->units[ i ]->solid = SOLID_BBOX;
	ent->units[ i ]->deadflag = DEAD_NO;
	ent->units[ i ]->clipmask = MASK_PLAYERSOLID;
	ent->units[ i ]->model = "players/male/tris.md2";
	ent->units[ i ]->waterlevel = 0;
	ent->units[ i ]->watertype = 0;
	ent->units[ i ]->flags &= ~FL_NO_KNOCKBACK;
	ent->units[ i ]->svflags &= ~SVF_DEADMONSTER;
	ent->units[ i ]->think = unit_think;
	ent->units[ i ]->nextthink = level.time + .1;

	VectorCopy (mins, ent->units[ i ]->mins);
	VectorCopy (maxs, ent->units[ i ]->maxs);
	VectorClear (ent->units[ i ]->velocity);

	ent->units[ i ]->s.effects = 0;
	ent->units[ i ]->s.modelindex = 255;
	ent->units[ i ]->s.modelindex2 = 255;
	ent->s.skinnum = ent - g_edicts - 1;

	ent->s.frame = 0;
	VectorCopy (spawn_origin, ent->units[ i ]->s.origin);
	ent->units[ i ]->s.origin[ 2 ] += 1;
	VectorCopy (ent->units[ i ]->s.origin, ent->units[ i ]->s.old_origin);

	ent->units[ i ]->s.angles[ YAW ] = spawn_angles[ YAW ];
	ent->units[ i ]->s.angles[ PITCH ] = 0;
	ent->units[ i ]->s.angles[ ROLL ] = 0;
	VectorCopy (ent->units[ i ]->s.angles, ent->client->ps.viewangles);
	VectorCopy (ent->units[ i ]->s.angles, ent->client->v_angle);

	gi.linkentity (ent->units[ i ]);
}