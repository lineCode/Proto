#include "proto/ComboBox.h"
#include "imgui.h"

proto::ComboBox::ComboBox(spvgentwo::IAllocator* _pAlloc, const char* _pTitle) : Vector(_pAlloc),
	m_pTitle(_pTitle),
	m_onSelect(_pAlloc)
{
}

proto::ComboBox::~ComboBox()
{
}

void proto::ComboBox::update()
{
    if (empty())
        return;

    m_selected = m_selected >= m_elements ? 0u : m_selected;
    const char* current_item = operator[](m_selected).c_str();

    if (ImGui::BeginCombo(m_pTitle, current_item))
    {
        for (int n = 0; n < m_elements; n++)
        {
            const spvgentwo::String& str = operator[](n);
            const char* item = str.c_str();

            bool is_selected = (current_item == item);

            if (ImGui::Selectable(item, is_selected))
            {
                m_selected = n;
                is_selected = true;
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
                if (m_onSelect)
                {
                    m_onSelect(m_selected, str);
                }
            }
        }
        ImGui::EndCombo();
    }
}
