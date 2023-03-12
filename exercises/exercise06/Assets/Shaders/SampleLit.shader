Shader "CG2023/SampleLit"
{
    Properties
    {
        _Albedo("Albedo", Color) = (1,1,1,1)
        _AlbedoTexture("Albedo Texture", 2D) = "white" {}
        _Reflectance("Reflectance (Ambient, Diffuse, Specular)", Vector) = (1, 1, 1, 0)
        _SpecularExponent("Specular Exponent", Float) = 100.0
    }

    SubShader
    {
        Tags { "RenderType" = "Opaque" }

        GLSLINCLUDE
        #include "UnityCG.glslinc"
        #include "ITUCG.glslinc"

        uniform vec4 _Albedo;
        uniform sampler2D _AlbedoTexture;
		// Albedo texture coordinate scale and offset
        uniform vec4 _AlbedoTexture_ST;
        uniform vec4 _Reflectance;
        uniform float _SpecularExponent;
        ENDGLSL

        Pass
        {
            Name "FORWARD"
            Tags { "LightMode" = "ForwardBase" }

            GLSLPROGRAM

            struct vertexToFragment
            {
                vec3 worldPos;
                vec3 normal;
                vec2 texCoords;
            };

            #ifdef VERTEX
            out vertexToFragment v2f;

            void main()
            {
                // Transform position and normal to world space
                v2f.worldPos = (unity_ObjectToWorld * gl_Vertex).xyz;
                v2f.normal = (unity_ObjectToWorld * vec4(gl_Normal, 0.0f)).xyz;

                // Transform texture coordinates with the Scale and Offset provided in the material
                v2f.texCoords = TransformTexCoords(gl_MultiTexCoord0.xy, _AlbedoTexture_ST);

                // Project vertex position
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            }
            #endif // VERTEX

            #ifdef FRAGMENT
            in vertexToFragment v2f;

            void main()
            {
                // Get light and view directions (L and V)
                vec3 lightDir = GetWorldSpaceLightDir(v2f.worldPos);
                vec3 viewDir = GetWorldSpaceViewDir(v2f.worldPos);

                // Normalize normal after linear interpolation
                vec3 normal = normalize(v2f.normal);

                // Sample albedo texture
                vec3 albedo = texture(_AlbedoTexture, v2f.texCoords).rgb;
                albedo *= _Albedo.rgb;

                // Compute lighting
                vec3 lighting = BlinnPhongLighting(lightDir, viewDir, normal, albedo, vec3(1.0f), _Reflectance.x, _Reflectance.y, _Reflectance.z, _SpecularExponent);

                gl_FragColor = vec4(lighting, 1.0f);
            }
            #endif // FRAGMENT

            ENDGLSL
        }
        Pass
        {
            Name "FORWARD"
            Tags { "LightMode" = "ForwardAdd" }

            ZWrite Off
            Blend One One

            GLSLPROGRAM

            struct vertexToFragment
            {
                vec3 worldPos;
                vec3 normal;
                vec2 texCoords;
            };

            #ifdef VERTEX
            out vertexToFragment v2f;

            void main()
            {
                v2f.worldPos = (unity_ObjectToWorld * gl_Vertex).xyz;
                v2f.normal = (unity_ObjectToWorld * vec4(gl_Normal, 0.0f)).xyz;
                v2f.texCoords = TransformTexCoords(gl_MultiTexCoord0.xy, _AlbedoTexture_ST);

                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            }
            #endif // VERTEX

            #ifdef FRAGMENT
            in vertexToFragment v2f;

            void main()
            {
                vec3 lightDir = GetWorldSpaceLightDir(v2f.worldPos);
                vec3 viewDir = GetWorldSpaceViewDir(v2f.worldPos);

                vec3 normal = normalize(v2f.normal);

                vec3 albedo = texture(_AlbedoTexture, v2f.texCoords).rgb;
                albedo *= _Albedo.rgb;

                // Compute lighting for the additional pass. Notice that here ambient reflectance (_Reflectance.x) is 0, to avoid adding ambient more than once.
                vec3 lighting = BlinnPhongLighting(lightDir, viewDir, normal, albedo, vec3(1.0f), 0.0f, _Reflectance.y, _Reflectance.z, _SpecularExponent);

                gl_FragColor = vec4(lighting, 1.0f);
            }
            #endif // FRAGMENT

            ENDGLSL
        }
        // ShadowCaster pass. This will be the shader executed to generate a shadow map.
        Pass
        {
            Name "SHADOWCASTER"
            Tags { "LightMode" = "ShadowCaster" }

            GLSLPROGRAM

            #ifdef VERTEX
            void main()
            {
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            }
            #endif // VERTEX

            #ifdef FRAGMENT
            void main()
            {
                // No output, since we are only using the output from depth
            }
            #endif // FRAGMENT

            ENDGLSL
        }
    }
}
