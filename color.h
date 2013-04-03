
#ifndef COLOR_H
#define COLOR_H


/**
 * @class Color
 *
 * Helper class that helps us to operate with colors with less clutter.
 */
class Color
{
private:
    // RGBA representation of the color.
    float m_color[4];

public:
    enum { R = 0, G, B, A };

public:
    /// Default constructor.
    Color()
    {
        m_color[R] = 0;
        m_color[G] = 0;
        m_color[B] = 0;
        m_color[A] = 1;
    }

    /// Copy constructor.
    Color(const float color[4])
    {
        for (unsigned int i = 0; i < 4; ++i) {
            if ((color[i] > 0) && (color[i] < 1))
                m_color[i] = color[i];
            else {
                m_color[i] = 0;
                if (i == 4)
                    m_color[i] = 1;
            }
        }
    }

    /// Copy constructor.
    Color(const Color &color)
    {
        m_color[R] = color[R];
        m_color[G] = color[G];
        m_color[B] = color[B];
        m_color[A] = color[A];
    }

    /// Copy constructor.
    Color(float red, float green, float blue, float alpha = 1)
    {
        m_color[R] = red;
        m_color[G] = green;
        m_color[B] = blue;
        m_color[A] = alpha;
    }

    /// Destructor.
    ~Color() {}

    /// Accessors

    float red() const { return m_color[R]; }
    float green() const { return m_color[G]; }
    float blue() const { return m_color[B]; }
    float alpha() const { return m_color[A]; }
    const float* rgba() const { return m_color; }

    /// Setters
    void setRed(float red) { m_color[R] = red; }
    void setGreen(float green) { m_color[G] = green; }
    void setBlue(float blue) { m_color[B] = blue; }
    void setAlpha(float alpha) { m_color[A] = alpha; }

    /// Overloaded operators

    inline Color & operator=(const Color &color)
    {
        m_color[R] = color.m_color[R];
        m_color[G] = color.m_color[G];
        m_color[B] = color.m_color[B];
        m_color[A] = color.m_color[A];
        return *this;
    }

    inline float& operator[](const unsigned index) { return m_color[index]; }
    inline float operator[](const unsigned index) const { return m_color[index]; }
};

// More on operators.

inline bool operator==(const Color &lhs, const Color &rhs)
{
    return (lhs.red() == rhs.red() && lhs.green() == rhs.green() 
            && lhs.blue() == rhs.blue() && lhs.alpha() == rhs.alpha());
}

inline bool operator!=(const Color &lhs, const Color &rhs)
{
    return !(lhs == rhs);
}

std::ostream & operator<<(std::ostream &os, const Color &c)
{
    os << c.red() << ", " << c.green() << ", " << c.blue() << ", " << c.alpha() << std::endl;
    return os;
}


#endif // COLOR_H
