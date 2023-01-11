--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type BP_Character_Base_C
local BP_Character_Base = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:UserConstructionScript()
-- end

function BP_Character_Base:ReceiveBeginPlay()
    self.Overridden.ReceiveBeginPlay(self)
    --self.BM_PlayerAttrComponent.OnMaxHealthChanged:Add(self, self.OnMaxHealthChanged)
    --self.BM_PlayerAttrComponent.OnHealthChanged:Add(self, self.OnHealthChanged)
    self.OnHpChanged:Add(self, self.OnCharacterHpChanged)
    --self:UpdateAttr()
end

function BP_Character_Base:OnCharacterHpChanged()
    print("OnHpChanged ....")
    self:UpdateAttr()
end

function BP_Character_Base:OnMaxHealthChanged(Instigator, OldValue, NewValue)
    print("OnMaxHealthChanged ....")
    self:UpdateAttrHp()
end

function BP_Character_Base:OnHealthChanged(Instigator, OldValue, NewValue)
    print("OnHealthChanged ....", OldValue, NewValue)
    self:UpdateAttrHp()
end

function BP_Character_Base:UpdateAttrHp()
    local AttrWidget = self:GetAttrWidget()
    if AttrWidget then
        local CurHpPercent = self:GetHpPercent()
        if CurHpPercent then
            AttrWidget:UpdateHp(CurHpPercent)
        end
    end
end

function BP_Character_Base:GetHpPercent()
    local MaxHp = self.CurMaxHp
    local CurHp = self.CurHp
    if MaxHp == 0 then
        print("MaxHp======0")
        return
    end
    print("MaxHp!!!!!!!======0")
    local HpPercent = CurHp / MaxHp
    return HpPercent
end

function BP_Character_Base:GetAttrWidget()
    local UP_PlayerAttr = self.AttrWidget:GetUserWidgetObject()
    return UP_PlayerAttr
end

--function BP_Character_Base:UpdateAttr()
--    local AttrWidget = self:GetAttrWidget()
--    if AttrWidget then
--        if self.CurPlayerState then
--            AttrWidget:UpdateName(self.CurPlayerState.PlayerName)
--        end
--        self:UpdateAttrHp()
--    end
--end

function BP_Character_Base:ReceiveEndPlay()
end

--function BP_Character_Base:ReceiveTick(DeltaSeconds)
--end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return BP_Character_Base
