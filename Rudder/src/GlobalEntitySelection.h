#ifndef __GLOBALENTITYSELECTION_H__
#define __GLOBALENTITYSELECTION_H__

#include <Rush.h>

class GlobalEntitySelection {
private:
    static Rush::Entity s_SelectedEntity;
public:
    GlobalEntitySelection() = delete;

    static void ClearSelection(){ s_SelectedEntity = Rush::Entity(); }
    static void SetSelection(Rush::Entity e){ s_SelectedEntity = e; }
    static bool IsSelection(Rush::Entity e){ return s_SelectedEntity == e; }
    static Rush::Entity GetSelection(){ return s_SelectedEntity; }
};

#endif // __GLOBALENTITYSELECTION_H__
