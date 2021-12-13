using System;
using System.Runtime.CompilerServices;

namespace Rush
{
    public static class Random
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetSeed(UInt32 seed);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern int GetInt();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern int GetInt(int max);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern int GetInt(int min, int max);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float GetFloat();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float GetFloat(float max);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float GetFloat(float min, float max);
    };

}