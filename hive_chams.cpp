#include "hive_cheats.h"
#include "hive_util.h"
IMaterial* materialChams;
IMaterial* materialChamsIgnorez;
IMaterial* materialChamsFlat;
IMaterial* materialChamsFlatIgnorez;
IMaterial* materialChamsArms;
IMaterial* materialChamsWeapons;
IMaterial* materialWireframe;
IMaterial* materialWireframeIgnorez;
void ForceMaterial(Color color, IMaterial* material)
{
		float blend[3] = { (float)color.r() / 255.f, (float)color.g() / 255.f, (float)color.b() / 255.f };
		float alpha = (float)color.a() / 255.f;
		CHiveInterface.Render->SetBlend(alpha);
		CHiveInterface.Render->SetColorModulation(blend);
		CHiveInterface.ModelRender->ForcedMaterialOverride(material);
		
}
void DrawPlayer(IVModelRender* _ModelRender, DrawModelState_t const& state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	CBaseEntityNew* localplayer = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(CHiveInterface.Engine->GetLocalPlayer());
	if (!localplayer)
		return;

	CBaseEntityNew* entity = (CBaseEntityNew*)CHiveInterface.EntityList->GetClientEntity(pInfo.entity_index);
	if (!entity
		|| NativeClass::IsDormant(entity)
		|| !entity->isAlive())
		return;

	if (entity == localplayer)
		return;

	IMaterial* visible_material;
	IMaterial* hidden_material;
	IMaterial* wireframe_material;
	IMaterial* hidden_wireframe_material;
		visible_material = materialChamsFlat;
		hidden_material = materialChamsFlatIgnorez;
		wireframe_material = materialWireframe;
		wireframe_material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		hidden_wireframe_material = materialWireframeIgnorez;
		wireframe_material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);

		Color hidden = Color(0, 128, 255, 255);
		Color visible = Color(255, 205, 0, 255);
		Color wireframe = Color(53, 98, 38, 255);
	if (CLuaMenuCallback.Chams) {
		ForceMaterial(hidden, hidden_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
		ForceMaterial(visible, visible_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
	}
	if (CLuaMenuCallback.ChamsWireframe) {
		ForceMaterial(wireframe, wireframe_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
		ForceMaterial(wireframe, hidden_wireframe_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);

	}
}

void DrawWeapon(IVModelRender* _ModelRender, DrawModelState_t const& state, const ModelRenderInfo_t &pInfo, matrix3x4_t* pCustomBoneToWorld) {
	IMaterial* visible_material;
	IMaterial* hidden_material;
	IMaterial* wireframe_material;
	IMaterial* hidden_wireframe_material;

	visible_material = materialChamsFlat;
	hidden_material = materialChamsFlatIgnorez;
	wireframe_material = materialWireframe;
	wireframe_material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	hidden_wireframe_material = materialWireframeIgnorez;
	wireframe_material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);

	Color hidden = Color(145, 0, 255, 255);
	Color visible = Color(111, 255, 0, 255);
	Color wireframe = Color(53, 98, 38, 255);
	if (CLuaMenuCallback.Chams) {
		ForceMaterial(hidden, hidden_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
		ForceMaterial(visible, visible_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
	}
	if (CLuaMenuCallback.ChamsWireframe) {
		ForceMaterial(wireframe, wireframe_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
		ForceMaterial(wireframe, hidden_wireframe_material);
		CHiveInterface.ModelRender->DrawModelExecute(state, pInfo, pCustomBoneToWorld);
	}
}

namespace HiveCheats {
	void Chams(IVModelRender* _ModelRender, DrawModelState_t const& DrawModelState, ModelRenderInfo_t const& ModelRenderInfo, matrix3x4_t *pCustomBoneToWorld) {

		static bool InitMaterials = false;
		if (!InitMaterials)
		{
			materialChams = HiveUTIL::CreateMaterial("VertexLitGeneric", "VGUI/white_additive", false, true, true, true, true);
			materialChamsIgnorez = HiveUTIL::CreateMaterial("VertexLitGeneric", "VGUI/white_additive", true, true, true, true, true);
			materialChamsFlat = HiveUTIL::CreateMaterial("UnlitGeneric", "VGUI/white_additive", false, true, true, true, true);
			materialChamsFlatIgnorez = HiveUTIL::CreateMaterial("UnlitGeneric", "VGUI/white_additive", true, true, true, true, true);
			materialWireframe = HiveUTIL::CreateMaterial("Wireframe", "VGUI/white_additive", false, true, true, true, true);
			materialWireframeIgnorez = HiveUTIL::CreateMaterial("Wireframe", "VGUI/white_additive", true, true, true, true, true);
			InitMaterials = true;
		}
		std::string modelName = CHiveInterface.ModelInfo->GetModelName(ModelRenderInfo.pModel);
		if (modelName.find("models/player") != std::string::npos) {
			DrawPlayer(_ModelRender, DrawModelState, ModelRenderInfo, pCustomBoneToWorld);
		}
		//IMatRenderContext
		/*
		CHiveInterface.MaterialSystem->GetRenderContext()->ClearBuffersObeyStencil(1,1);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilEnable(true);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilWriteMask(255);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilTestMask(255);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilReferenceValue(1);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilCompareFunction(STENCILCOMPARISONFUNCTION_ALWAYS);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilPassOperation(STENCILOPERATION_REPLACE);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilFailOperation(STENCILOPERATION_KEEP);
		CHiveInterface.MaterialSystem->GetRenderContext()->SetStencilZFailOperation(STENCILOPERATION_REPLACE);
		*/

		if (modelName.find("weapon") != std::string::npos) {
			DrawWeapon(_ModelRender, DrawModelState, ModelRenderInfo, pCustomBoneToWorld);
		}
	}
}