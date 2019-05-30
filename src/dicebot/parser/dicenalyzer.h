#pragma once

#include <deque>
#include <list>
#include <memory>
#include <vector>
#include "../utils/number.h"
#include "./syntax.h"

namespace dicebot::diceparser {
    enum class cal_err { fine, negative_dice, dice_num_exceed, dice_face_exceed, div_zero };

    using str_container = std::stringstream;
    using result_container = std::deque<number>;

    template <class item_t>
    using parser_pointer = std::shared_ptr<item_t>;

    class component {
    public:
        virtual number roll_the_dice(str_container &) const = 0;
        virtual void print(str_container &) const noexcept = 0;
        virtual ~component() = default;
    };

    class base_holder {};

    using p_component = parser_pointer<component>;

    class comp_number : public component {
    public:
        number what;
        number roll_the_dice(str_container &) const override;
        void print(str_container &) const noexcept override;
    };
    using p_number = parser_pointer<comp_number>;

    class comp_holder : public component, public base_holder {
    public:
        p_component child;
        number roll_the_dice(str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    enum class dice_rdk_mode { single_d, numbered_d, numbered_d_k, numbered_d_kl };

    class comp_dice_rdk : public component {
    public:
        dice_rdk_mode mode;
        number dice, face, keep;
        number roll_the_dice(str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class comp_calculus : public component {
    public:
        p_component lchild;
        p_component rchild;
        char what;
        number roll_the_dice(str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class comp_calculus_reverse : public component {
    public:
        p_component child;
        number roll_the_dice(str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class dicelet : public component {
    public:
        number roll_the_dice(str_container &) const override { return 0; }
        virtual void roll_dicelet(result_container &, str_container &) const = 0;
    };
    using p_dicelet = parser_pointer<dicelet>;

    class dicelet_unit_sharp : public dicelet {
    public:
        number count;
        p_component child;
        void roll_dicelet(result_container &, str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class dicelet_unit_brace : public dicelet {
    public:
        std::vector<p_component> dicelets;
        void roll_dicelet(result_container &, str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class dicelet_calculus : public dicelet {
    public:
        p_dicelet lchild;
        p_dicelet rchild;
        char what;
        void roll_dicelet(result_container &, str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class dicelet_holder : public dicelet, public base_holder {
    public:
        p_dicelet child;
        void roll_dicelet(result_container &, str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    class dicelet_calculus_reverse : public dicelet {
    public:
        p_dicelet child;
        char what;
        void roll_dicelet(result_container &, str_container &) const override;
        void print(str_container &) const noexcept override;
    };

    p_component build_component_from_syntax(const syntax_item *root);
} // namespace dicebot::diceparser
