/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** sparse_array.hpp
*/


#pragma once

#include <vector>
#include <optional>
#include <memory>
#include <stdexcept>


template <typename Component>
class sparse_array {
public:
    using value_type = Component;
    using reference_type = std::optional<value_type>&;
    using const_reference_type = const std::optional<value_type>&;
    using container_t = std::vector<std::optional<value_type>>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

public:
    sparse_array() = default;
    sparse_array(const sparse_array& other) = default;
    sparse_array(sparse_array&& other) noexcept = default;
    ~sparse_array() = default;
    sparse_array& operator=(const sparse_array& other) = default;
    sparse_array& operator=(sparse_array&& other) noexcept = default;

    // Access
    reference_type operator[](size_t idx) {
        if (idx < _data.size())
            return _data[idx];
        else {
            int size = (idx>0 ? idx : idx*-1);
            _data.resize(size + 1);
            return _data[idx];
        }
    }

    const_reference_type operator[](size_t idx) const {
        if (idx >= _data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return _data[idx];
    }

    // const_reference_type operator[](size_t idx) const {
    //     if (idx < _data.size())
    //         return _data[idx];
    //     else {
    //         just_a_corrector(idx);
    //         return _data[idx];
    //     }
    // }

    // Iterattors
    iterator begin() { return _data.begin(); }
    const_iterator begin() const { return _data.begin(); }
    const_iterator cbegin() const { return _data.cbegin(); }
    iterator end() { return _data.end(); }
    const_iterator end() const { return _data.end(); }
    const_iterator cend() const { return _data.cend(); }

    // Capacity
    size_type size() const {
        return _data.size();
    }


    reference_type insert_at(size_type pos, const Component& component) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = component;
        return _data[pos];
    }

    reference_type insert_at(size_type pos, Component&& component) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = std::move(component);
        return _data[pos];
    }

    template <class... Params>
    reference_type emplace_at(size_type pos, Params&&... params) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos].emplace(std::forward<Params>(params)...);
        return _data[pos];
    }

    void erase(size_type pos) {
        if (pos < _data.size()) {
            _data[pos] = std::nullopt;
        }
    }

    size_type get_index(const value_type& component) const {
        for (size_type i = 0; i < _data.size(); ++i) {
            if (_data[i].has_value() && std::addressof(_data[i].value()) == std::addressof(component)) {
                return i;
            }
        }
        throw std::runtime_error("Component not found in sparse_array");
        //return static_cast<size_type>(-1);
    }

    bool has_component(size_t index) const {
        return index < _data.size() && _data[index].has_value();
    }

private:
    container_t _data;
};

