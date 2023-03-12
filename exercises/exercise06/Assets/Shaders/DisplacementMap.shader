Shader "CG2023/DisplacementMap"
{
    Properties
    {
        _Albedo("Albedo", Color) = (1,1,1,1)
        _AlbedoTexture("Albedo Texture", 2D) = "white" {}
        _NormalTexture("Normal Texture", 2D) = "normal" {}
        _DisplacementDistance("Displacement Distance", Float) = 0.1
        _DisplacementTexture("Displacement Texture", 2D) = "black" {}
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
        uniform vec4 _AlbedoTexture_ST;
        uniform sampler2D _NormalTexture;
        uniform vec4 _NormalTexture_ST;
        uniform vec4 _Reflectance;
        uniform float _SpecularExponent;

        uniform float _DisplacementDistance;
        uniform sampler2D _DisplacementTexture;
        uniform vec4 _DisplacementTexture_ST;
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
                vec4 tangent;
                vec4 texCoords;
            };

            #ifdef VERTEX
            attribute vec4 Tangent;

            out vertexToFragment v2f;

            void main()
            {
                v2f.worldPos = (unity_ObjectToWorld * gl_Vertex).xyz;
                v2f.normal = (unity_ObjectToWorld * vec4(gl_Normal, 0.0f)).xyz;
                v2f.tangent = vec4((unity_ObjectToWorld * vec4(Tangent.xyz, 0.0f)).xyz, Tangent.w);
                v2f.texCoords.xy = TransformTexCoords(gl_MultiTexCoord0.xy, _AlbedoTexture_ST);
                v2f.texCoords.zw = TransformTexCoords(gl_MultiTexCoord0.xy, _NormalTexture_ST);

                // Sample the displacement texture
                vec2 texCoordsDisplacement = TransformTexCoords(gl_MultiTexCoord0.xy, _DisplacementTexture_ST);
                float displacement = texture(_DisplacementTexture, texCoordsDisplacement).r - 0.5f;

                // Multiply displacement by the distance
                displacement *= _DisplacementDistance;

                // Offset world position in the direction of the normal
                v2f.worldPos += displacement * normalize(v2f.normal);

                // IMPORTANT: We have to compute the projection with the modified world position!
                gl_Position = unity_MatrixVP * vec4(v2f.worldPos, 1.0f);
            }
            #endif // VERTEX

            #ifdef FRAGMENT
            in vertexToFragment v2f;

            void main()
            {
                vec3 lightDir = GetWorldSpaceLightDir(v2f.worldPos);
                vec3 viewDir = GetWorldSpaceViewDir(v2f.worldPos);

                vec3 normal = normalize(v2f.normal);

                vec3 normalTS = textureNormal(_NormalTexture, v2f.texCoords.zw);
                mat3 tangentToWorld = GetTangentToWorldMatrix(normal, v2f.tangent);
                normal = tangentToWorld * normalTS;

                vec3 albedo = texture(_AlbedoTexture, v2f.texCoords.xy).rgb;
                albedo *= _Albedo.rgb;

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
                vec4 tangent;
                vec4 texCoords;
            };

            #ifdef VERTEX
            attribute vec4 Tangent;

            out vertexToFragment v2f;

            void main()
            {
                v2f.worldPos = (unity_ObjectToWorld * gl_Vertex).xyz;
                v2f.normal = (unity_ObjectToWorld * vec4(gl_Normal, 0.0f)).xyz;
                v2f.tangent = vec4((unity_ObjectToWorld * vec4(Tangent.xyz, 0.0f)).xyz, Tangent.w);
                v2f.texCoords.xy = TransformTexCoords(gl_MultiTexCoord0.xy, _AlbedoTexture_ST);
                v2f.texCoords.zw = TransformTexCoords(gl_MultiTexCoord0.xy, _NormalTexture_ST);

                // Sample the displacement texture
                vec2 texCoordsDisplacement = TransformTexCoords(gl_MultiTexCoord0.xy, _DisplacementTexture_ST);
                float displacement = texture(_DisplacementTexture, texCoordsDisplacement).r - 0.5f;

                // Multiply displacement by the distance
                displacement *= _DisplacementDistance;

                // Offset world position in the direction of the normal
                v2f.worldPos += displacement * normalize(v2f.normal);

                // IMPORTANT: We have to compute the projection with the modified world position!
                gl_Position = unity_MatrixVP * vec4(v2f.worldPos, 1.0f);
            }
            #endif // VERTEX

            #ifdef FRAGMENT
            in vertexToFragment v2f;

            void main()
            {
                vec3 lightDir = GetWorldSpaceLightDir(v2f.worldPos);
                vec3 viewDir = GetWorldSpaceViewDir(v2f.worldPos);

                vec3 normal = normalize(v2f.normal);

                vec3 normalTS = textureNormal(_NormalTexture, v2f.texCoords.zw);
                mat3 tangentToWorld = GetTangentToWorldMatrix(normal, v2f.tangent);
                normal = tangentToWorld * normalTS;

                vec3 albedo = texture(_AlbedoTexture, v2f.texCoords.xy).rgb;
                albedo *= _Albedo.rgb;

                vec3 lighting = BlinnPhongLighting(lightDir, viewDir, normal, albedo, vec3(1.0f), 0.0f, _Reflectance.y, _Reflectance.z, _SpecularExponent);
                gl_FragColor = vec4(lighting, 1.0f);
            }
            #endif // FRAGMENT

            ENDGLSL
        }
        Pass
        {
            Name "ShadowCaster"
            Tags { "LightMode" = "ShadowCaster" }

            GLSLPROGRAM

            #ifdef VERTEX
            void main()
            {
                vec3 worldPos = (unity_ObjectToWorld * gl_Vertex).xyz;
                vec3 normal = (unity_ObjectToWorld * vec4(gl_Normal, 0.0f)).xyz;

                // Sample the displacement texture
                vec2 texCoordsDisplacement = TransformTexCoords(gl_MultiTexCoord0.xy, _DisplacementTexture_ST);
                float displacement = texture(_DisplacementTexture, texCoordsDisplacement).r - 0.5f;

                // Multiply displacement by the distance
                displacement *= _DisplacementDistance;

                // Offset world position in the direction of the normal
                worldPos += displacement * normalize(normal);

                // IMPORTANT: We have to compute the projection with the modified world position!
                gl_Position = unity_MatrixVP * vec4(worldPos, 1.0f);
            }
            #endif // VERTEX

            #ifdef FRAGMENT
            void main()
            {
            }
            #endif // FRAGMENT

            ENDGLSL
        }
    }
}

