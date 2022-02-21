class Scout_CFTools extends Scout_Base {
    private bool _isTestingEnvironment = false;

    void Scout_CFTools() {
        string tmp;
        if(GetGameLabs().GetConfiguration().GetDebugStatus() && GetGame().CommandlineGetParam("gamelabstesting", tmp)) {
            this._isTestingEnvironment = true;
        }
    }

    override float GetChanceToJam() {
        if(!this._isTestingEnvironment) return super.GetChanceToJam();
        return 0.0;
    }

    override bool CanPutInCargo(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutInCargo(parent);

        PlayerBase player;
        if(parent.GetHierarchyRootPlayer() && PlayerBase.CastTo(player, parent.GetHierarchyRootPlayer())) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutIntoHands(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutIntoHands(parent);

        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override bool CanPutAsAttachment(EntityAI parent) {
        if(!this._isTestingEnvironment) return super.CanPutAsAttachment(parent);

        PlayerBase player;
        if(parent && PlayerBase.CastTo(player, parent)) {
            if(!player.HasAnyIdentitySet()) return true;
            if(player._GLSIA()) return true;
            else {
                this.Delete();
                return false;
            }
        }
        return false;
    }

    override void EEFired(int muzzleType, int mode, string ammoType) {
        super.EEFired(muzzleType, mode, ammoType);

        if(!this._isTestingEnvironment) return;

        Magazine magazine;
        if(GetGame().IsServer()) {
            magazine = this.GetMagazine(this.GetCurrentMuzzle());
            if(GetGame().IsMultiplayer() && magazine) GetGame().RemoteObjectDelete(magazine);
            if(magazine) magazine.ServerSetAmmoMax();
            if(GetGame().IsMultiplayer() && magazine) GetGame().RemoteObjectCreate(magazine);
        } else {
            magazine = this.GetMagazine(this.GetCurrentMuzzle());
            if(magazine) magazine.LocalSetAmmoMax();
        }
    }

    override void OnDebugSpawn() {
        GameInventory inventory = GetInventory();
        inventory.CreateInInventory( "ACOGOptic" );

        SpawnAttachedMagazine("Mag_Scout_5Rnd");
    }
};