#include "proto/TypedDataInput.h"
#include "spvgentwo/Constant.h"
#include "imgui.h"

using namespace spvgentwo;

proto::TypedDataInput::TypedDataInput(const char* _label) :
	m_label(_label),
	m_min{},
	m_max{},
	m_step{},
	m_fastStep{},
	m_data{}
{
}

proto::TypedDataInput::~TypedDataInput()
{
}

bool proto::TypedDataInput::update(const Type& _type)
{
	int dim = 0u;
	ImGuiDataType_ type = ImGuiDataType_COUNT;

	if (_type.isScalar())
	{
		dim = 1u;
	}
	else if (_type.isVector())
	{
		dim = _type.getVectorComponentCount();
	}
	else if (_type.isMatrix())
	{
		dim = _type.getMatrixColumnCount() * _type.front().getVectorComponentCount();
	}
	else
	{
		return false;
	}

	const Type& base = _type.getBaseType();

	if (base.isS16())
	{		
		type = ImGuiDataType_S16;
		m_step.s16 = m_step.s16 == 0 ? 1 : m_step.s16;
	}
	else if (base.isS32())
	{
		type = ImGuiDataType_S32;
		m_step.s32 = m_step.s32 == 0 ? 1 : m_step.s32;
	}
	else if (base.isS64())
	{
		type = ImGuiDataType_S64;
		m_step.s64 = m_step.s64 == 0 ? 1 : m_step.s64;
	}
	else if (base.isU16())
	{
		type = ImGuiDataType_U16;
		m_step.u16 = m_step.u16 == 0 ? 1 : m_step.u16;
	}
	else if (base.isU32())
	{
		type = ImGuiDataType_U32;
		m_step.u32 = m_step.u32 == 0 ? 1 : m_step.u32;
	}
	else if (base.isU64())
	{
		type = ImGuiDataType_U64;
		m_step.u64 = m_step.u64 == 0 ? 1 : m_step.u64;
	}
	else if (base.isF32())
	{
		type = ImGuiDataType_Float;
		m_step.f32 = m_step.f32 == 0.f ? 1.f : m_step.f32;
	}
	else if (base.isF64())
	{
		type = ImGuiDataType_Double;
		m_step.f64 = m_step.f64 == 0.0 ? 1.0 : m_step.f64;
	}
	else
	{
		return false;
	}

	if (m_inputDrag)
	{
		ImGui::DragScalarN(m_label, type, &m_data, dim, m_speed, &m_min, &m_max, m_format, m_power);
	}
	if (m_inputScalar)
	{
		ImGui::InputScalarN(m_label, type, &m_data, dim, &m_step, &m_fastStep);
	}
	if (m_inputColor && type == ImGuiDataType_Float)
	{
		if (dim == 3)
		{
			ImGui::ColorEdit3(m_label, m_data.f32v3);
		}
		else if (dim == 4)
		{
			ImGui::ColorEdit4(m_label, m_data.f32v4);
		}
	}

	if (ImGui::Button("Input options"))
	{	
		m_showOptions = !m_showOptions;
	}

	if (m_showOptions) // CollapsingHeader
	{
		ImGui::RadioButton("Drag", &m_activeOption, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Scalar", &m_activeOption, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Color", &m_activeOption, 2);

		m_inputDrag = m_activeOption == 0;
		m_inputScalar = m_activeOption == 1;
		m_inputColor = m_activeOption == 2;

		if (m_inputDrag)
		{
			ImGui::InputScalar("Min", type, &m_min);
			ImGui::InputScalar("Max", type, &m_max);
			ImGui::InputFloat("Speed", &m_speed);
			ImGui::InputFloat("Power", &m_power);
		}

		if (m_inputScalar)
		{
			ImGui::InputScalar("Step", type, &m_step);
			ImGui::InputScalar("FastStep", type, &m_fastStep);
		}
	}

	return true;
}

bool proto::TypedDataInput::update(const spvgentwo::Type& _type, spvgentwo::Constant& _outConstant)
{
	if (update(_type) == false)
	{
		return false;	
	}

	// integral scalar types

	if (_type.isS16())
	{
		_outConstant.make(m_data.s16);
	}
	else if (_type.isS32())
	{
		_outConstant.make(m_data.s32);
	}
	else if (_type.isS64())
	{
		_outConstant.make(m_data.s64);
	}
	else if (_type.isU16())
	{
		_outConstant.make(m_data.u16);
	}
	else if (_type.isU32())
	{
		_outConstant.make(m_data.u32);
	}
	else if (_type.isU64())
	{
		_outConstant.make(m_data.u64);
	}

	// floats
	else if (_type.isF32())
	{
		_outConstant.make(m_data.f32);
	}
	else if (_type.isF64())
	{
		_outConstant.make(m_data.f64);
	}

	// signed int 16 vec
	else if (_type.isVectorOfSInt(2u, 16u))
	{
		_outConstant.make(make_vector(m_data.s16v2));
	}
	else if (_type.isVectorOfSInt(3u, 16u))
	{
		_outConstant.make(make_vector(m_data.s16v3));
	}
	else if (_type.isVectorOfSInt(4u, 16u))
	{
		_outConstant.make(make_vector(m_data.s16v4));
	}

	// unsinged int 16 vec
	else if (_type.isVectorOfUInt(2u, 16u))
	{
		_outConstant.make(make_vector(m_data.u16v2));
	}
	else if (_type.isVectorOfUInt(3u, 16u))
	{
		_outConstant.make(make_vector(m_data.u16v3));
	}
	else if (_type.isVectorOfUInt(4u, 16u))
	{
		_outConstant.make(make_vector(m_data.u16v4));
	}

	// signed int 32 vec
	else if (_type.isVectorOfSInt(2u, 32u))
	{
		_outConstant.make(make_vector(m_data.s32v2));
	}
	else if (_type.isVectorOfSInt(3u, 32u))
	{
		_outConstant.make(make_vector(m_data.s32v3));
	}
	else if (_type.isVectorOfSInt(4u, 32u))
	{
		_outConstant.make(make_vector(m_data.s32v4));
	}

	// unsinged int 32 vec
	else if (_type.isVectorOfUInt(2u, 32u))
	{
		_outConstant.make(make_vector(m_data.u32v2));
	}
	else if (_type.isVectorOfUInt(3u, 32u))
	{
		_outConstant.make(make_vector(m_data.u32v3));
	}
	else if (_type.isVectorOfUInt(4u, 32u))
	{
		_outConstant.make(make_vector(m_data.u32v4));
	}

	// signed int 64 vec
	else if (_type.isVectorOfSInt(2u, 64u))
	{
		_outConstant.make(make_vector(m_data.s64v2));
	}
	else if (_type.isVectorOfSInt(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.s64v3));
	}
	else if (_type.isVectorOfSInt(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.s64v4));
	}

	// unsinged int 64 vec
	else if (_type.isVectorOfUInt(2u, 64u))
	{
		_outConstant.make(make_vector(m_data.u64v2));
	}
	else if (_type.isVectorOfUInt(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.u64v3));
	}
	else if (_type.isVectorOfUInt(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.u64v4));
	}

	// float vec 32
	else if (_type.isVectorOfFloat(2u, 32u))
	{
		_outConstant.make(make_vector(m_data.f32v2));
	}
	else if (_type.isVectorOfFloat(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.f32v3));
	}
	else if (_type.isVectorOfFloat(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.f32v4));
	}

	// float vec 64
	else if (_type.isVectorOfFloat(2u, 64u))
	{
		_outConstant.make(make_vector(m_data.f64v2));
	}
	else if (_type.isVectorOfFloat(3u, 64u))
	{
		_outConstant.make(make_vector(m_data.f64v3));
	}
	else if (_type.isVectorOfFloat(4u, 64u))
	{
		_outConstant.make(make_vector(m_data.f64v4));
	}

	// s16 mat
	else if (_type.isMatrixOfInt(2u, 2u, 16u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s16m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 16u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s16m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 16u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s16m44));
	}

	// u16 mat
	else if (_type.isMatrixOfInt(2u, 2u, 16u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u16m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 16u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u16m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 16u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u16m44));
	}


	// s32 mat
	else if (_type.isMatrixOfInt(2u, 2u, 32u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s32m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 32u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s32m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 32u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s32m44));
	}

	// u32 mat
	else if (_type.isMatrixOfInt(2u, 2u, 32u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u32m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 32u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u32m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 32u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u32m44));
	}

	// s64 mat
	else if (_type.isMatrixOfInt(2u, 2u, 64u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s64m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 64u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s64m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 64u, Sign::Signed))
	{
		_outConstant.make(make_matrix(m_data.s64m44));
	}

	// u64 mat
	else if (_type.isMatrixOfInt(2u, 2u, 64u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u64m22));
	}
	else if (_type.isMatrixOfInt(3u, 3u, 64u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u64m33));
	}
	else if (_type.isMatrixOfInt(4u, 4u, 64u, Sign::Unsigned))
	{
		_outConstant.make(make_matrix(m_data.u64m44));
	}

	// f32mat
	else if (_type.isMatrixOfFloat(2u, 2u, 32u))
	{
		_outConstant.make(make_matrix(m_data.f32m22));
	}
	else if (_type.isMatrixOfFloat(3u, 3u, 32u))
	{
		_outConstant.make(make_matrix(m_data.f32m33));
	}
	else if (_type.isMatrixOfFloat(4u, 4u, 32u))
	{
		_outConstant.make(make_matrix(m_data.f32m44));
	}

	// f64 mat
	else if (_type.isMatrixOfFloat(2u, 2u, 64u))
	{
		_outConstant.make(make_matrix(m_data.f64m22));
	}
	else if (_type.isMatrixOfFloat(3u, 3u, 64u))
	{
		_outConstant.make(make_matrix(m_data.f64m33));
	}
	else if (_type.isMatrixOfFloat(4u, 4u, 64u))
	{
		_outConstant.make(make_matrix(m_data.f64m44));
	}

	else // could not match type
	{
		return false;
	}

	return _outConstant.getType() == _type;
}