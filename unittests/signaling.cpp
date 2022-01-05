#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "entt/signal/delegate.hpp"
#include "entt/signal/sigh.hpp"
#include "entt/signal/dispatcher.hpp"
#include "entt/signal/emitter.hpp"

#include <iostream>
#include <fmt/format.h>

int f(int const& i)
{
    return i;
}

TEST_CASE("delegate")
{
    struct Food
    {
        int f(int const& i) const { return i; }
    };

    {
        // its like std::function but not heap allocs
        entt::delegate<int(int)> delegate{};

        // Bind free function
        delegate.connect<&f>();
        auto res = delegate(10);
        REQUIRE(10 == res);
    }

    {
        // its like std::function but not heap allocs
        entt::delegate<int(int)> delegate{};

        Food foo;
        // Bind to struct fun
        delegate.connect<&Food::f>(foo);

        auto res = delegate(10);
        REQUIRE(10 == res);
    }

    {
        Food foo;
        entt::delegate delegate{
                +[](const void* ptr, int value) { return static_cast<const Food*>(ptr)->f(value); },
                &foo};
    }
}

namespace sig
{

void foo(int i, char c)
{
    std::cout << fmt::format("foo(int({}), char({})\n", i, c);
}

struct listener
{
    void bar(int const& i, char c) { std::cout << fmt::format("foo(int({}), char({})\n", i, c); }
};

int f()
{
    return 2;
}
int g()
{
    return 4;
}

TEST_CASE("signals")
{
    {
        entt::sigh<void(int, char)> signal;

        entt::sink sink{signal};
        listener instance;
        sink.connect<&foo>();
        sink.connect<&listener::bar>(instance);

        signal.publish(42, 'c');

        sink.disconnect<&foo>();
        sink.disconnect<&listener::bar>(instance);

        // disconnect all of this instance
        sink.disconnect(instance);

        // disconnect all
        sink.disconnect();
    }

    {
        entt::sigh<int()> signal;
        entt::sink sink{signal};

        sink.connect<&f>();
        sink.connect<&g>();

        std::vector<int> vec;
        signal.collect([&vec](auto value) { vec.push_back(value); });

        REQUIRE(2 == vec.at(0));
        REQUIRE(4 == vec.at(1));
    }
}

} // namespace sig

namespace events
{

struct FirstEvent
{
    int i = 22;
};
struct SecondEvent
{
    int i = 44;
};

struct Listener
{
    void receive(FirstEvent const& e) { std::cout << fmt::format("FirstEvent({})\n", e.i); }
    void method(SecondEvent const& e) { std::cout << fmt::format("SecondEvent({})\n", e.i); }
};

TEST_CASE("events")
{
    {
        // Immediate dispatch example for urgent messages
        entt::dispatcher dispatcher{};

        Listener listener;
        dispatcher.sink<FirstEvent>().connect<&Listener::receive>(listener);
        dispatcher.sink<SecondEvent>().connect<&Listener::method>(listener);

        dispatcher.trigger(FirstEvent{.i = 42});
        dispatcher.trigger(SecondEvent{.i = 420});

        dispatcher.sink<FirstEvent>().disconnect<&Listener::receive>(listener);
        dispatcher.sink<SecondEvent>().disconnect(listener);
    }

    {
        // Queued dispatch, dispatch once per tick
        entt::dispatcher dispatcher{};

        Listener listener;
        dispatcher.sink<FirstEvent>().connect<&Listener::receive>(listener);
        dispatcher.sink<SecondEvent>().connect<&Listener::method>(listener);

        // Queue up some events
        dispatcher.enqueue(FirstEvent{.i = 69});
        dispatcher.enqueue(SecondEvent{.i = 690});

        // Sends all messages
        dispatcher.update();

        // Send only this type
        dispatcher.update<FirstEvent>();

        dispatcher.sink<FirstEvent>().disconnect<&Listener::receive>(listener);
        dispatcher.sink<SecondEvent>().disconnect(listener);
    }
}

} // namespace events

namespace emitters
{

struct Event
{
    int payload = 99;
};

struct MyEmitter : entt::emitter<MyEmitter>
{
    int id = 1;
};

struct Listener
{
    void handle(Event const& event)
    {
        std::cout << fmt::format("Event({})\n", event.payload);
    }
};

TEST_CASE("event emitter")
{
    {
        MyEmitter emitter;
        Listener listener;

        // Register longlived listener, meant to be sent event more than once
        auto conn = emitter.on<Event>([&listener](Event const& event, MyEmitter const& emitter) {
            std::cout << fmt::format(
                    "Registered connection to emitter({}) object with event({})\n",
                    emitter.id,
                    event.payload);

            listener.handle(event);
        });

        // Check if there is anyone listening
        if(emitter.empty<Event>())
        {
        }

        emitter.publish<Event>(987);

        // No need to manually clean when intended to go out of scope
        emitter.erase(conn);
    }
}

} // namespace emitters
