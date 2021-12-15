using System;
using System.Runtime.CompilerServices;
using GlmSharp;

namespace Rush
{
    public class TransformComponent : Component
    {
        private TransformComponent() { }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern mat4 GetModelMatrix();

        public extern vec3 Translation
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern quat Rotation
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
        public extern vec3 Scale
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void Translate(vec3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void DoScale(vec3 scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void Rotate(quat rotation);
    }

}