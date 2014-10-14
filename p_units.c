#include "g_local.h"
#include "m_player.h"

void unit_RunFrames (edict_t *ent, int start, int end)
{
	if (ent->s.frame < end)
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
	int i;

	for (i = 0; i < 3; i++)
	{
		if (ent->s.old_origin[ i ] != ent->s.origin[ i ])
		{
			return false;
		}
	}

	unit_RunFrames (ent, FRAME_stand01, FRAME_stand40);

	return true;
}

void unit_die (edict_t *ent, edict_t *owner)
{
}

void unit_pain (edict_t *ent)
{
	unit_RunFrames (ent, FRAME_pain101, FRAME_pain104);
}

void unit_damage (edict_t *ent, int damage)
{
}

void unit_move (edict_t *ent)
{
	if (ent->deadflag)
	{
		return;
	}

	VectorCopy (ent->s.old_origin, ent->s.origin);
	VectorCopy (ent->owner->s.origin, ent->s.origin);
	VectorCopy (ent->owner->s.angles, ent->s.angles);

	if (!unit_stand (ent))
	{
		ent->owner->MP--;
		unit_RunFrames (ent, FRAME_run1, FRAME_run6);
	}
}

void initUnit (edict_t *ent, int i)
{
	vec3_t	spawn_origin, spawn_angles;
	vec3_t	mins = {-16, -16, -24};
	vec3_t	maxs = {16, 16, 32};

	ent->units[ i ] = G_Spawn ();
	SelectSpawnPoint (ent, spawn_origin, spawn_angles);

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