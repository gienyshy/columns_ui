#include "stdafx.h"
#include "buttons.h"

CLIPFORMAT ButtonsToolbar::ConfigParam::ButtonsList::g_clipformat()
{
    static auto cf = (CLIPFORMAT)RegisterClipboardFormat(L"CUIListViewStandardClipFormat");
    return cf;
}

void ButtonsToolbar::ConfigParam::ButtonsList::notify_on_initialisation()
{
    set_single_selection(true);

    pfc::list_t<uih::ListView::Column> columns;
    columns.add_item(uih::ListView::Column("Name", 300));
    columns.add_item(uih::ListView::Column("Type", 150));

    set_columns(columns);
}
void ButtonsToolbar::ConfigParam::ButtonsList::notify_on_create()
{
    pfc::com_ptr_t<ButtonsListDropTarget> IDT_blv = new ButtonsListDropTarget(this);
    RegisterDragDrop(get_wnd(), IDT_blv.get_ptr());
}
void ButtonsToolbar::ConfigParam::ButtonsList::notify_on_destroy()
{
    RevokeDragDrop(get_wnd());
}
void ButtonsToolbar::ConfigParam::ButtonsList::notify_on_selection_change(
    const pfc::bit_array& p_affected, const pfc::bit_array& p_status, notification_source_t p_notification_source)
{
    t_size index = get_selected_item_single();
    m_param.on_selection_change(index);
}

bool ButtonsToolbar::ConfigParam::ButtonsList::do_drag_drop(WPARAM wp)
{
    UINT cf = g_clipformat();
    mmh::ComPtr<IDataObject> pDO = new CDataObject;

    DDData data = {0, get_wnd()};
    uih::ole::set_blob(pDO, cf, &data, sizeof(data));

    DWORD drop_effect = DROPEFFECT_NONE;
    uih::ole::do_drag_drop(get_wnd(), wp, pDO, DROPEFFECT_MOVE, NULL, &drop_effect);

    return true;
}
