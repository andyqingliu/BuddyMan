--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type UP_PlayerAttr_C
local UP_PlayerAttr = UnLua.Class()

function UP_PlayerAttr:Construct()
    print("UP_PlayerAttr Construct ....")
end

function UP_PlayerAttr:Destruct()

end

function UP_PlayerAttr:UpdateName(Name)
    self.Name:SetText(Name)
    self.PlayerAttrName = Name
    print("UP_PlayerAttr UpdateName = ", Name)
end

function UP_PlayerAttr:UpdateCurHp(Character)
    local MaxHp = Character.BM_PlayerAttrComponent:GetCurrentMaxHp()
    local CurHp = Character.BM_PlayerAttrComponent:GetCurrentHp()
    print("<<<<<<<<UP_PlayerAttr Name, UpdateCurHp = ", Character.PlayerNameNew, MaxHp, CurHp)
    if MaxHp == 0 then
        return
    end
    local HpPercent = CurHp / MaxHp
    self:UpdateHp(HpPercent)
end

function UP_PlayerAttr:UpdateHp(Index, PercentHp)
    self.WidgetSwitcher_Hp:SetActiveWidgetIndex(Index)
    local HpIndex = Index + 1
    self["Hp_".. HpIndex]:SetPercent(PercentHp)
    print("UP_PlayerAttr Name, UpdateHp = ", self.PlayerAttrName, PercentHp)
end

function UP_PlayerAttr:ReceiveEndPlay(EndPlayReason)
    print("UP_PlayerAttr:ReceiveEndPlay", EndPlayReason)
end



return UP_PlayerAttr
