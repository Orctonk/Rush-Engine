using System;
using System.Runtime.CompilerServices;

namespace Rush
{
    public static class Logger
    {
        public enum Level
        {
            Error = 0,
            Warning = 1,
            Info = 2,
            Trace = 3,

        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Log(Level logLevel, string message);

        public static void Error(string fmt, params object[] list)
        {
            Log(Level.Error, String.Format(fmt, list));
        }
        public static void Warning(string fmt, params object[] list)
        {
            Log(Level.Warning, String.Format(fmt, list));
        }
        public static void Info(string fmt, params object[] list)
        {
            Log(Level.Info, String.Format(fmt, list));
        }
        public static void Trace(string fmt, params object[] list)
        {
            Log(Level.Trace, String.Format(fmt, list));
        }
    }
}