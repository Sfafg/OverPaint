#pragma once
#include <vector>
#include <iostream>
#include <typeinfo>

template <typename T>
class SpanSet
{
public:
    struct Bounds
    {
        T lower;
        T upper;
        bool lowerIn;
        bool upperIn;

        Bounds(T lower) : lower(lower), upper(lower), lowerIn(true), upperIn(true) {}
        Bounds(T lower, T upper, bool lowerIn = true, bool upperIn = true) : lower(lower), upper(upper), lowerIn(lowerIn), upperIn(upperIn) {}
        Bounds(T lower, bool lowerIn, T upper, bool upperIn) : lower(lower), upper(upper), lowerIn(lowerIn), upperIn(upperIn) {}

        bool IsIntersecting(Bounds other) const
        {
            if (other.upper < lower) return false;
            if (other.upper == lower && !lowerIn && !other.upperIn) return false;
            if (other.lower > upper) return false;
            if (other.lower == upper && !upperIn && !other.lowerIn) return false;
            return true;
        }
        bool Contains(T element) const
        {
            if (lower < element && element < upper)return true;
            if (lower == element && lowerIn) return true;
            if (upper == element && upperIn) return true;
            return false;
        }

        void Merge(Bounds other)
        {
            if (!IsIntersecting(other)) return;

            if (other.lower < lower)
            {
                lower = other.lower;
                lowerIn = other.lowerIn;
            }
            else if (other.lower == lower && other.lowerIn)
                lowerIn = true;

            if (other.upper > upper)
            {
                upper = other.upper;
                upperIn = other.upperIn;
            }
            else if (other.upper == upper && other.upperIn)
                upperIn = true;
        }

        Bounds Remove(Bounds other)
        {
            T up = upper;
            bool upIn = upperIn;
            Bounds extra(0, 0, false, false);
            if (other.lower > lower)
            {
                if (other.upper < upper)
                {
                    extra.lower = other.upper;
                    extra.upperIn = !other.upperIn;
                }
                upper = other.lower;
                upperIn = !other.lowerIn;
            }

            if (other.upper < up)
            {
                if (other.lower > lower)
                {
                    extra.upper = up;
                    extra.upperIn = upIn;
                }
                else
                {
                    lower = other.upper;
                    lowerIn = !other.upperIn;
                }
            }

            return extra;
        }

        bool Valid() const
        {
            if constexpr (std::is_integral<T>::value)
            {
                if (abs(lower - upper) <= 1 && !lowerIn && !upperIn) return false;
            }
            if (lower < upper) return true;
            if (lower == upper && lowerIn && upperIn) return true;
            return false;
        }
    };

public:
    SpanSet()
    {}

    SpanSet& Add(Bounds bounds)
    {
        int index = -1;
        for (int i = 0; i < m_spans.size(); i++)
        {
            if (m_spans[i].IsIntersecting(bounds))
            {
                m_spans[i].Merge(bounds);
                index = i;
                break;
            }
        }
        if (index == -1)
        {
            m_spans.push_back(bounds);
            return *this;
        }

        for (int j = index + 1; j < m_spans.size(); j++)
        {
            if (m_spans[index].IsIntersecting(m_spans[j]))
            {
                m_spans[index].Merge(m_spans[j]);
                m_spans.erase(m_spans.begin() + j);
            }
        }
        for (int j = index - 1; j >= 0; j--)
        {
            if (m_spans[index].IsIntersecting(m_spans[j]))
            {
                m_spans[index].Merge(m_spans[j]);
                m_spans.erase(m_spans.begin() + j);
                index--;
            }
        }

        return *this;
    }
    SpanSet& Remove(Bounds bounds)
    {
        for (int i = m_spans.size() - 1; i >= 0; i--)
        {
            if (m_spans[i].IsIntersecting(bounds))
            {
                Bounds b = m_spans[i].Remove(bounds);
                if (b.Valid())
                    m_spans.push_back(b);

                if (!m_spans[i].Valid())
                    m_spans.erase(m_spans.begin() + i);
            }
        }

        return *this;
    }

    bool Contains(T element) const
    {
        for (int i = 0; i < m_spans.size(); i++)
        {
            if (m_spans[i].Contains(element))
            {
                return true;
            }
        }
        return false;
    }
    std::vector<Bounds>::iterator begin()
    {
        return m_spans.begin();
    }
    std::vector<Bounds>::iterator end()
    {
        return m_spans.end();
    }
    const std::vector<Bounds>::const_iterator begin() const
    {
        return m_spans.cbegin();
    }
    const std::vector<Bounds>::const_iterator end() const
    {
        return m_spans.cend();
    }
    const int size() const
    {
        return m_spans.size();
    }

private:
    std::vector<Bounds> m_spans;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const SpanSet<T>& rhs)
{
    for (int i = 0; i < rhs.size(); i++)
    {
        auto bounds = rhs.begin()[i];

        os << (bounds.lowerIn ? "[" : "(") << bounds.lower;
        os << ", ";
        os << bounds.upper << (bounds.upperIn ? "]" : ")");
        os << " ";
    }
    return os;
}