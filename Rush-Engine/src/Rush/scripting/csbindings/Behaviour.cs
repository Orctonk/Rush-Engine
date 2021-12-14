using System;
using System.Runtime.CompilerServices;
using GlmSharp;

namespace Rush
{
    class Behaviour : Component
    {
        public Behaviour() { }

        public void Update()
        {
            vec2 v = new vec2(Random.GetFloat(), Random.GetFloat());
            if (Input.KeyDown(Key.A))
            {
                Logger.Info("hej {0}", v);
            }
        }
    }
}
