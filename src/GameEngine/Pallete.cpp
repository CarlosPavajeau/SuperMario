#include "Pallete.h"
#include "GameEngine.h"

namespace GameEngine
{
	Pallete::Pallete()
	{

	}

	void Pallete::Create(const std::initializer_list<sf::Color>& original_colors, const std::initializer_list<sf::Color>& swaped_colors)
	{
        assert(original_colors.size() == swaped_colors.size());
        int arr_size = original_colors.size();
        const sf::String frag_shader =
            "#version 120\n"\
            "const int arr_size = " + ToString(arr_size) + ";"\
            "uniform vec3 color1[arr_size];"\
            "uniform vec3 color2[arr_size];"\
            "uniform sampler2D texture;"\
            "void main()"\
            "{"\
            "vec4 myindex = texture2D(texture, gl_TexCoord[0].xy);"\
            "vec3 index = vec3(myindex.r,myindex.g,myindex.b);"\
            "gl_FragColor = myindex;"\
            "for (int i=0; i < arr_size; ++i)"\
            "if (index == color1[i]) {"\
            "gl_FragColor = vec4(color2[i].r,color2[i].g,color2[i].b,myindex.a);"\
            "break; }"\
            "}";
        _shader.loadFromMemory(frag_shader, sf::Shader::Fragment);
        auto colors1 = new sf::Glsl::Vec3[arr_size];
        auto colors2 = new sf::Glsl::Vec3[arr_size];
        int i = 0;
        for (auto original_color : original_colors)
            colors1[i++] = sf::Glsl::Vec3(original_color.r / 255.f, original_color.g / 255.f, original_color.b / 255.f);
        i = 0;
        for (auto swaped_color : swaped_colors)
            colors2[i++] = sf::Glsl::Vec3(swaped_color.r / 255.f, swaped_color.g / 255.f, swaped_color.b / 255.f);
        _shader.setUniformArray("color1", (sf::Glsl::Vec3*)colors1, arr_size);
        _shader.setUniformArray("color2", (sf::Glsl::Vec3*)colors2, arr_size);
        _shader.setUniform("texture", sf::Shader::CurrentTexture);
        delete[] colors1;
        delete[] colors2;
	}

    void Pallete::Apply()
    {
        sf::Shader::bind(&_shader);
    }

    void Pallete::Cancel()
    {
        sf::Shader::bind(nullptr);
    }
}