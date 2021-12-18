#ifndef __EXCEPTIONMANAGER_H__
#define __EXCEPTIONMANAGER_H__

#include <mono/metadata/object.h>

namespace Rush {

struct Exception {
    std::string message;
    std::string function;
    std::string file;
    int line;
};

class ExceptionManager {
private:
    ExceptionManager() = delete;

    static std::vector<Exception> s_Exceptions;

    static MonoObject *GetPropertyObject(MonoObject *object, std::string propertyName);
    static std::string GetPropertyString(MonoObject *object, std::string propertyName);
    static void ParseStackTrace(std::string stackTrace, Exception &e);

public:
    static void HandleException(MonoObject *exception);

    static const std::vector<Exception> &GetExceptionList() { return s_Exceptions; }
    static void ClearExceptionList() { s_Exceptions.clear(); }
};

} // namespace Rush

#endif // __EXCEPTIONMANAGER_H__
