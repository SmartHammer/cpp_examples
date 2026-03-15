#pragma once

#include <vector>

namespace observer
{
    template <typename... Args>
    class signal
    {
    public:
        using slot_type = void (*)(Args...);

        void connect(slot_type slot)
        {
            slots_.push_back({.slot = slot, .blocked = false});
        }

        void emit(Args... args)
        {
            for (auto &slot : slots_)
            {
                if (not slot.blocked)
                {
                    slot.slot(args...);
                }
            }
        }

        void disconnect_all()
        {
            slots_.clear();
        }

        void block(slot_type slot)
        {
            for (auto &s : slots_)
            {
                if (s.slot == slot)
                {
                    s.blocked = true;
                }
            }
        }

        void unblock(slot_type slot)
        {
            for (auto &s : slots_)
            {
                if (s.slot == slot)
                {
                    s.blocked = false;
                }
            }
        }

    private:
        struct Observer
        {
            slot_type slot;
            bool blocked{false};
        };

        std::vector<Observer> slots_;
    };
} // namespace observer
