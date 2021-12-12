using System;
using System.Runtime.CompilerServices;

namespace Rush
{
    class Behaviour : Component
    {
        UInt64 lastUpdate = 0;

        public Behaviour() { }

        public void Update()
        {
            if (Time.ProgramTimeMillis() - lastUpdate > 1000)
            {
                Logger.Info("hej {0}", Time.Delta());
                lastUpdate = Time.ProgramTimeMillis();
            }
        }
    }
}
