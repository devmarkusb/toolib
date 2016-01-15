// Markus Borris, 2015
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef BASIC_DESIGN_H_IMCL_dshfine87tn37ts23tns83tns37st
#define BASIC_DESIGN_H_IMCL_dshfine87tn37ts23tns83tns37st

#error Do not include this header file! The idea is to use it just as a source for copy&paste.

#include "../../../../ToolsFL/tooBasicCodeSupport/include/Toolib/std/std_extensions.h"


namespace too
{

    //! Demonstrating constructors, destructor, assignments - all six, or none of them.
    struct SomeClass
    {
        SomeClass() = default;
        //! You should either list none of the following five methods, or all of them.
        /** If you specify them, then you can do this by implementation, =default or =delete.*/
        ~SomeClass() = default; // should be noexcept, if implemented
        SomeClass(const SomeClass&) = default;
        SomeClass(SomeClass&& other) = default; // should be noexcept, if implemented
        SomeClass& operator=(const SomeClass& other) = default;
        // example, if implementation needed
        /**
    {
        SomeClass tmp(other);
        *this = std::move(tmp);
        return *this;
    }
        */
        SomeClass& operator=(SomeClass&&) = default; // should be noexcept, if implemented
    };

    //! Typical setup for an abstract interface, e.g. you don't want to copy anything virtual.
    /** Also make sure, that you don't templatize a class designated to be a base.*/
    struct AbstractInterface
    {
        virtual ~AbstractInterface() = default;
        AbstractInterface() = default; // yes, deleting would prevent derived classes from being constructed
        // not so sure about these; cf. example too::any, where I neeed all of them unmentioned (not even defaulted)
//        AbstractInterface(const AbstractInterface&) = delete;
//        AbstractInterface(AbstractInterface&& other) = delete;
//        AbstractInterface& operator=(const AbstractInterface&) = delete;
//        AbstractInterface& operator=(AbstractInterface&&) = delete;

        //! Note that using a smart pointer, inheriting classes cannot change the return type.
        //! In some ('covariant') cases they could want to. Then they should be able to.
        //! It should be 'clear', that a clone-function returns ownership.
        virtual AbstractInterface* clone() = 0;
    };

    //! Note that in this case you need to provide an out-of-class implementation of the destructor.
    struct AbstractInterface_without_functions
    {
        virtual inline ~AbstractInterface() = 0;
        AbstractInterface() = default;
        AbstractInterface(const AbstractInterface&) = delete;
        AbstractInterface(AbstractInterface&& other) = delete;
        AbstractInterface& operator=(const AbstractInterface&) = delete;
        AbstractInterface& operator=(AbstractInterface&&) = delete;
    };
    inline AbstractInterface_without_functions::~AbstractInterface_without_functions() {}

    //! Factory function (alternatively being part of a factory class).
    template <class T, typename... ConstructorArgs>
    std::unique_ptr<T> create(ConstructorArgs... args)
    {
        return std::make_unique<T>(args...);
    }

    struct Impl1 : public AbstractInterface
    {
        virtual ~Impl1() = default; // or implement
        Impl1() = default; // or implement
        Impl1(const Impl1&) = delete;
        Impl1(Impl1&& other) = delete;
        Impl1& operator=(const Impl1&) = delete;
        Impl1& operator=(Impl1&&) = delete;

        //! note that using a smart pointer you could only write std::unique_ptr<AbstractInterface*> non-covariantly
        virtual Impl1* clone() override { return nullptr; /*todo implement*/ }
    };

    //! If a constructor is not enough.
    struct ClassWithComplexInit
    {
        virtual ClassWithComplexInit* create()
        {
            ClassWithComplexInit* ret = nullptr;
            try
            {
                ret = new ClassWithComplexInit;
                // ...possibly virtual method calls...
            }
            catch (...)
            {
            }
            return ret;
        }
        virtual ~SomeClass() = default; // or implement
        SomeClass(const SomeClass&) = delete;
        SomeClass(SomeClass&& other) = delete;
        SomeClass& operator=(const SomeClass& other) = delete;
        SomeClass& operator=(SomeClass&&) = delete;

    protected:
        ClassWithComplexInit() = default; // or implement, possibly noexcept(false)
    };

}

#endif
