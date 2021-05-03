#pragma once



namespace Project001
{
    struct BaseComponent
    {
        BaseComponent()
            : entityId((unsigned int)-1)
        {}

        unsigned int entityId;
    };

    template <unsigned int uniqueComponentTypeId>
    struct Component : BaseComponent
    {
        const static unsigned int typeId = uniqueComponentTypeId;
    };

    // Example:
    // ----------------------------------------------------------------------------
    // * Components must inherit from the Component class.

    /// struct TestComponent : Component<0>
    /// {
    ///     TestComponent(int a, int b, int c)
    ///         : a(a)
    ///         , b(b)
    ///         , c(c)
    ///     {}
    ///
    ///     ~TestComponent()
    ///     {}
    ///
    ///     int a;
    ///     int b;
    ///     int c;
    /// };
}