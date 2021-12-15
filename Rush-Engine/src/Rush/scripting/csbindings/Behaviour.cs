using System;
using System.Runtime.CompilerServices;
using GlmSharp;

namespace Rush
{
    class Behaviour : Component
    {
        private TransformComponent tc;

        public Behaviour()
        {
            tc = this.parent.GetComponent<TransformComponent>();
        }

        public void Update()
        {
            if (Input.KeyDown(Key.A))
                tc.Translation = tc.Translation + new vec3(-0.05f, 0.0f, 0.0f);
            if (Input.KeyDown(Key.D))
                tc.Translation = tc.Translation + new vec3(0.05f, 0.0f, 0.0f);
            if (Input.KeyDown(Key.S))
                tc.Translation = tc.Translation + new vec3(0.0f, -0.05f, 0.0f);
            if (Input.KeyDown(Key.W))
                tc.Translation = tc.Translation + new vec3(-0.01f, 0.05f, 0.0f);

        }
    }
}
