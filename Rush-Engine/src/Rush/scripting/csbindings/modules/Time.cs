using System;
using System.Runtime.CompilerServices;

namespace Rush
{
    public static class Time
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern double Delta();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern double FixedDelta();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern double TimeScale();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern UInt64 ProgramTimeMillis();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern UInt64 ProgramTimeMicros();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetTimeScale(double timescale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetFixedDelta(double fixeddelta);
    };
}