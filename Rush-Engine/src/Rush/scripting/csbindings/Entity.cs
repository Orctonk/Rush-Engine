using System;
using System.Runtime.CompilerServices;

namespace Rush
{
    public class Entity
    {
#pragma warning disable 0169
        private IntPtr registry;
        private UInt32 entityHandle;
#pragma warning restore 0169
        private Entity() { }

        // [MethodImplAttribute(MethodImplOptions.InternalCall)]
        // private extern static LightComponent GetComponent_LightComponent();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern TransformComponent GetComponent_TransformComponent();

        public T GetComponent<T>() where T : Component
        {
            if (typeof(T) == typeof(TransformComponent))
                return (T)(Component)GetComponent_TransformComponent();
            return null;
        }
    }
}
