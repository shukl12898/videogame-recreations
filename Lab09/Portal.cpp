#include "Portal.h"
#include "Game.h"
#include "Actor.h"
#include "PortalMeshComponent.h"

Portal::Portal(Game* game)
: Actor(game)
{
	mPortalMesh = new PortalMeshComponent(this);
}