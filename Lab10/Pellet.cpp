#include "Actor.h"
#include "Pellet.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.h"

Pellet::Pellet(Game* game)
: Actor(game)
{
	mGame = game;
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Meshes/Sphere.gpmesh");
	mMeshComponent->SetMesh(mesh);
    mMeshComponent->SetTextureIndex(1);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(25.0f, 25.0f, 25.0f);
}

void Pellet::OnUpdate(float deltaTime)
{
    //Pellet should travel 500 units in current movement direction
    SetPosition(GetPosition()+ mForward*SPEED*deltaTime);
    mLifetime += deltaTime;

    if (mCollisionComponent->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>())){
        SetState(ActorState::Destroy);
    }

    if (mLifetime >= 0.25){
        std::vector<Actor*> colliders = mGame->GetColliders();
        for (int i = 0; i < colliders.size(); i++){
            if (mCollisionComponent->Intersect(colliders[i]->GetComponent<CollisionComponent>())){
                SetState(ActorState::Destroy);
            }
        }
    }

}