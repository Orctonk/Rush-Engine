using System;
using System.Runtime.CompilerServices;
using GlmSharp;

namespace Rush
{
    class Behaviour : Component
    {
        private TransformComponent tc;
        private LightComponent lc;

        public Behaviour()
        {
            tc = parent.GetComponent<TransformComponent>();
            lc = parent.GetComponent<LightComponent>();
        }

        public void Update()
        {
            UInt64 time = Time.ProgramTimeMillis();

            vec3 translation = tc.Translation;
            translation.x = glm.Cos(time / 1000.0f) * 2;
            translation.y = glm.Sin(time / 1000.0f) * 2;
            tc.Translation = translation;

            lc.Range = 100;

            vec3[] cols = new vec3[5] {
                new vec3(238, 96, 85),
                new vec3(96, 211, 148),
                new vec3(170, 246, 131),
                new vec3(255, 217, 125),
                new vec3(255, 155, 133)
            };
            for (int i = 0; i < 5; i++)
            {
                cols[i] /= 255;
            }

            float t = (time % 1000.0f) / 1000.0f;
            int colIdx = (int)((time / 1000) % 5);
            vec3 col1 = cols[colIdx];
            vec3 col2 = cols[(colIdx + 1) % 5];
            lc.Diffuse = new vec3((1 - t) * col1 + t * col2);
        }
    }
}
