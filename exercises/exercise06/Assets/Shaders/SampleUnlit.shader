Shader "CG2023/SampleUnlit"
{
    // Material properties
    Properties
    {
        // Texture 2D property: If none is specified, it will use a white texture
        _MainTex ("Texture", 2D) = "white" {}
    }

    // Each Shader can have several subshaders, to use depending on the platform and quality settings
    // We only use one
    SubShader
    {
        // Tags for the rendering system. This will label this shader as being opaque. It doesn't change the shader
        Tags { "RenderType"="Opaque" }

        // Shader lab files can contain several passes, to render the object more than once if needed
        // This shader only has one pass (only rendered once)
        Pass
        {
            GLSLPROGRAM // Start of GLSL code block

            uniform sampler2D _MainTex;

            // Vertex shader specific code
            #ifdef VERTEX

            // Texture coordinates output variable to be interpolated to the fragment shader
            out vec2 texCoords;

            // Starting point for vertex shader
            void main()
            {
                texCoords = gl_MultiTexCoord0.xy;

                // Multiply vertex position attribute (in model space) by model-view-projection matrix
                // Assign to predefined gl_Position used by the rendering pipeline
                gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            }
            #endif

            // Fragment shader specific code
            #ifdef FRAGMENT

            // Texture coordinates input variable interpolated from the vertex shader
            // Name must match the 'out' variable declared in vertex shader
            in vec2 texCoords;

            // Starting point for fragment shader
            void main()
            {
                // Sample texture _MainTex in the specified coordinates and assign it as the final fragment color
                gl_FragColor = texture(_MainTex, texCoords);
            }
            #endif

            ENDGLSL // End of GLSL code block
        }
    }
}
