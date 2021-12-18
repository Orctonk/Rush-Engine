#ifndef __EXCEPTIONPANEL_H__
#define __EXCEPTIONPANEL_H__

class ExceptionPanel {
public:
    bool enabled;

    ExceptionPanel();
    ~ExceptionPanel();

    void OnImguiRender();
};

#endif // __EXCEPTIONPANEL_H__
