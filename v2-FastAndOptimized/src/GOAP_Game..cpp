#include <stdio.h>
#include "GOAP_Core.h"
#include "GOAP_Soldier.h"

int main()
{
    // 1. Ustvarimo SoldierActor (deduje iz GOAP_Actor in ima vse akcije + cilje)
    SoldierActor npc;

    // 2. Kličemo GOAP planner
    npc.GeneratePlan();

    // 3. (Opcijsko) - lahko spreminjamo stanje sveta in generiramo nov plan
    // npc.SetWorldState(BowEquipped | EnemyInSight);
    // npc.GeneratePlan();

    return 0;
}