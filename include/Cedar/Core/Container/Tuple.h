//
// Created by itshu on 17/6/2024.
//

#ifndef CEDAR_CORE_CONTAINER_TUPLE_H
#define CEDAR_CORE_CONTAINER_TUPLE_H

namespace Cedar::Core::Container {
    template<typename... Ts>
    class Tuple;

    template<>
    class Tuple<> {};

    template<typename T, typename... Ts>
    class Tuple<T, Ts...> : public Tuple<Ts...> {
        T value;
    public:
        Tuple(T val, Ts... vals) : Tuple<Ts...>(vals...), value(val) {}

        T getValue() { return value; }

        template<int index>
        auto get() {
            if constexpr (index == 0) {
                return value;
            } else {
                return Tuple<Ts...>::template get<index - 1>();
            }
        }
    };
}

#endif //CEDAR_CORE_CONTAINER_TUPLE_H
