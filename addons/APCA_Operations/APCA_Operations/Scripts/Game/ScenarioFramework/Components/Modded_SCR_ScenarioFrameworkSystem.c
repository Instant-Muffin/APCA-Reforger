modded class SCR_ScenarioFrameworkSystem : GameSystem
{	
		//------------------------------------------------------------------------------------------------
	//! Goes over despawned areas and checks whether or not said area should spawn
	override void DynamicSpawn()
	{
		for (int i = m_aDespawnedAreas.Count() - 1; i >= 0; i--)
		{
			Tuple3<SCR_ScenarioFrameworkArea, vector, int> areaInfo = m_aDespawnedAreas[i];
			foreach (vector observerPos : m_aObservers)
			{
				if (vector.DistanceSqXZ(observerPos, areaInfo.param2) < areaInfo.param3)
				{
					if(areaInfo.param1.GetExcludeAir())
					{
						if(observerPos[1] - GetGame().GetWorld().GetSurfaceY(observerPos[0], observerPos[2]) < 3)
						{
							Print("APCA Test - In Air In Zone");
							areaInfo.param1.DynamicReinit();
							m_aSpawnedAreas.Insert(areaInfo);
							m_aDespawnedAreas.Remove(i);
							break;
						};
					}
					else
					{
						Print("APCA Test - In Zone");
						areaInfo.param1.DynamicReinit();
						m_aSpawnedAreas.Insert(areaInfo);
						m_aDespawnedAreas.Remove(i);
						break;
					}
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Goes over spawned areas and checks whether or not said area should despawn
	override void DynamicDespawn()
	{
		for (int i = m_aSpawnedAreas.Count() - 1; i >= 0; i--)
		{
			Tuple3<SCR_ScenarioFrameworkArea, vector, int> areaInfo = m_aSpawnedAreas[i];
			bool observerInRange;
			foreach (vector observerPos : m_aObservers)
			{
				if (vector.DistanceSqXZ(observerPos, areaInfo.param2) < areaInfo.param3)
				{
					if(areaInfo.param1.GetExcludeAir())
					{
						if(observerPos[1] - GetGame().GetWorld().GetSurfaceY(observerPos[0], observerPos[2]) < 3)
						{
							observerInRange = true;
							break;
						}
					}
					else
					{
						observerInRange = true;
						break;
					}
				}
			}

			if (!observerInRange)
			{
				areaInfo.param1.DynamicDespawn(null);
				m_aDespawnedAreas.Insert(areaInfo);
				m_aSpawnedAreas.Remove(i);
			}
		}
	}
	
}
