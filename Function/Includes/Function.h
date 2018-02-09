#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>

#include "Functor\FreeFunctionFunctor.h"
#include "Functor\MemberFunctionFunctor.h"
#include "Functor\MemberConstFunctionFunctor.h"

namespace fn
{
	// Non specialized template declaration for a function (empty)
	template <typename>
	class Function;

	// Specialization for a function
	template <typename TReturn, typename... TArgs>
	class Function<TReturn(TArgs...)> final
	{
	private:
		using TFunctor = fnu::Functor<TReturn(TArgs...)>;
		using TFunctorSharedPtr = std::shared_ptr<TFunctor>;

	public:
		// Creates an unbound (empty) function
		Function();

		// Creates a function from a free/static function
		Function(TReturn(*freeFunction)(TArgs...));

		// Creates a function from a member function
		template <typename TClass>
		Function(TReturn(TClass::*memberFunction)(TArgs...), TClass* instance);

		// Creates a function from a member const function
		template <typename TClass>
		Function(TReturn(TClass::*memberFunction)(TArgs...) const, TClass* instance);

		// Resets this function to its unboud state (empty)
		void reset();

		// Resets this function to a given function's state
		void reset(Function* function);

		// Resets this function to a new free/static function
		void reset(TReturn(*freeFunction)(TArgs...));

		// Resets this function to a new member function
		template <typename TClass>
		void reset(TReturn(TClass::*memberFunction)(TArgs...), TClass* instance);

		// Resets this function to a new member const function
		template <typename TClass>
		void reset(TReturn(TClass::*memberConstFunction)(TArgs...) const, TClass* instance);

		// Invokes this function
		TReturn operator()(TArgs... args);
		
		// Checks whether both delegates point to the same function
		bool operator==(const Function<TReturn(TArgs...)>& function);

	private:
		TFunctorSharedPtr m_functor;
	};

	// Utility builder function for free functions
	template <typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)> bind(TReturn(*freeFunction)(TArgs...));

	// Utility builder function for member functions
	template <typename TClass, typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)> bind(TReturn(TClass::*memberFunction)(TArgs...), TClass* instance);

	// Utility builder function for member const functions
	template <typename TClass, typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)> bind(TReturn(TClass::*memberConstFunction)(TArgs...) const, TClass* instance);
}

#include "..\Inlines\Function.inl"

#endif