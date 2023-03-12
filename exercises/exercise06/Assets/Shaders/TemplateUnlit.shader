Shader "CG2023/TemplateUnlit"
{
    Properties
    {
    }

    SubShader
    {
        Tags { "RenderType"="Opaque" }

        GLSLINCLUDE
        #include "UnityCG.glslinc"
        #include "ITUCG.glslinc"
        ENDGLSL

        Pass
        {
            GLSLPROGRAM

            #ifdef VERTEX
            void main()
            {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            }
            #endif

            #ifdef FRAGMENT
            void main()
            {
                gl_FragColor = vec4(1.0f);
            }
            #endif

            ENDGLSL
        }
    }
}

