#ifndef FUNCTION_IMPL
#define FUNCTION_IMPL

namespace fn
{
	template <typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)>::Function()
	{
		reset();
	}

	template <typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)>::Function(TReturn(*freeFunction)(TArgs...))
	{
		reset(freeFunction);
	}

	template <typename TReturn, typename... TArgs>
	template <typename TClass>
	Function<TReturn(TArgs...)>::Function(TReturn(TClass::*memberFunction)(TArgs...), TClass* instance)
	{
		reset(memberFunction, instance);
	}

	template <typename TReturn, typename... TArgs>
	template <typename TClass>
	Function<TReturn(TArgs...)>::Function(TReturn(TClass::*memberConstFunction)(TArgs...) const, TClass* instance)
	{
		reset(memberConstFunction, instance);
	}

	template <typename TReturn, typename... TArgs>
	void Function<TReturn(TArgs...)>::reset()
	{
		m_functor.reset();
	}

	template <typename TReturn, typename... TArgs>
	void Function<TReturn(TArgs...)>::reset(Function* function)
	{
		m_functor = function->m_functor;
	}

	template <typename TReturn, typename... TArgs>
	void Function<TReturn(TArgs...)>::reset(TReturn(*freeFunction)(TArgs...))
	{
		m_functor.reset(new fnu::FreeFunctionFunctor<TReturn(TArgs...)>(freeFunction));
	}

	template <typename TReturn, typename... TArgs>
	template <typename TClass>
	void Function<TReturn(TArgs...)>::reset(TReturn(TClass::*memberFunction)(TArgs...), TClass* instance)
	{
		m_functor.reset(new fnu::MemberFunctionFunctor<TReturn(TClass::*)(TArgs...)>(memberFunction, instance));
	}

	template <typename TReturn, typename... TArgs>
	template <typename TClass>
	void Function<TReturn(TArgs...)>::reset(TReturn(TClass::*memberConstFunction)(TArgs...) const, TClass* instance)
	{
		m_functor.reset(new fnu::MemberConstFunctionFunctor<TReturn(TClass::*)(TArgs...) const>(memberConstFunction, instance));
	}

	template <typename TReturn, typename... TArgs>
	TReturn Function<TReturn(TArgs...)>::operator()(TArgs... args)
	{
		if (!m_functor)
			throw std::exception("Unbound function");

		return m_functor->operator()(std::forward<TArgs>(args)...);
	}

	template <typename TReturn, typename... TArgs>
	bool Function<TReturn(TArgs...)>::operator==(const Function<TReturn(TArgs...)>& function)
	{
		TFunctor* self = m_functor.get();
		TFunctor* target = function.m_functor.get();

		bool areBothEmpty = self == nullptr && target == nullptr;
		bool areBothValid = self != nullptr && target != nullptr;
		bool areBothEqual = areBothValid && (*self == *target);

		return areBothEmpty || areBothEqual;
	}

	template <typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)> bind(TReturn(*freeFunction)(TArgs...)) {
		return std::move(Function<TReturn(TArgs...)>(freeFunction));
	}

	// Utility builder function for member functions
	template <typename TClass, typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)> bind(TReturn(TClass::*memberFunction)(TArgs...), TClass* instance) {
		return std::move(Function<TReturn(TArgs...)>(memberFunction, instance));
	}

	// Utility builder function for member const functions
	template <typename TClass, typename TReturn, typename... TArgs>
	Function<TReturn(TArgs...)> bind(TReturn(TClass::*memberConstFunction)(TArgs...) const, TClass* instance) {
		return std::move(Function<TReturn(TArgs...)>(memberConstFunction, instance));
	}
}

#endif