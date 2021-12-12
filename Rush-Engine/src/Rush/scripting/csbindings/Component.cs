using System;

namespace Rush
{
    public class Component
    {
        public IntPtr native;

        public Entity parent { get; private set; }
    }
}