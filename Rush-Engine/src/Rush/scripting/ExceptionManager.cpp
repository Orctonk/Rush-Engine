#include "ExceptionManager.h"

#include <mono/metadata/appdomain.h>
#include <mono/metadata/exception.h>

namespace Rush {

std::vector<Exception> ExceptionManager::s_Exceptions;

MonoObject *ExceptionManager::GetPropertyObject(MonoObject *object, std::string propertyName) {
    MonoClass *c = mono_object_get_class(object);
    MonoProperty *msgProp = mono_class_get_property_from_name(c, propertyName.c_str());
    return mono_property_get_value(msgProp, object, NULL, NULL);
}

std::string ExceptionManager::GetPropertyString(MonoObject *object, std::string propertyName) {
    MonoObject *str = GetPropertyObject(object, propertyName);
    return mono_string_to_utf8((MonoString *)str);
}

void ExceptionManager::ParseStackTrace(std::string stackTrace, Exception &e) {
    std::string topLine = stackTrace.substr(0, stackTrace.find("\n"));
    size_t start = topLine.find("at ") + 3;
    size_t funcEnd = topLine.find(") [") + 1;
    size_t fileStart = topLine.find(" in ") + 4;
    size_t lineMarker = topLine.find_last_of(":");

    e.function = topLine.substr(start, funcEnd - start);
    e.file = topLine.substr(fileStart, lineMarker - fileStart);
    std::string line = topLine.substr(lineMarker + 1);
    e.line = std::stoi(line);
}

void ExceptionManager::HandleException(MonoObject *exception) {
    Exception e;
    e.message = GetPropertyString(exception, "Message");
    ParseStackTrace(GetPropertyString(exception, "StackTrace"), e);
    s_Exceptions.push_back(e);
}

} // namespace Rush