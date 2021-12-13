using System;
using System.Runtime.CompilerServices;

namespace Rush
{
    class Behaviour : Component
    {
        public Behaviour() { }

        public void Update()
        {
            if (Input.KeyDown(Key.A))
            {
                Logger.Info("hej {0}", Random.GetInt(0, 100));
            }
        }
    }
}
