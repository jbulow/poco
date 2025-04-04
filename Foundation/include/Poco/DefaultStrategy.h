//
// DefaultStrategy.h
//
// Library: Foundation
// Package: Events
// Module:  DefaultStrategy
//
// Implementation of the DefaultStrategy template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DefaultStrategy_INCLUDED
#define Foundation_DefaultStrategy_INCLUDED


#include "Poco/NotificationStrategy.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {


template <class TArgs, class TDelegate>
class DefaultStrategy: public NotificationStrategy<TArgs, TDelegate>
	/// Default notification strategy.
	///
	/// Internally, a std::vector<> is used to store
	/// delegate objects. Delegates are invoked in the
	/// order in which they have been registered.
{
public:
	using DelegateHandle = TDelegate*;
	using DelegatePtr = SharedPtr<TDelegate>;
	using Delegates = std::vector<DelegatePtr>;
	using Iterator = typename Delegates::iterator;

public:
	DefaultStrategy() = default;

	DefaultStrategy(const DefaultStrategy& s):
		_delegates(s._delegates)
	{
	}

	~DefaultStrategy() = default;

	void notify(const void* sender, TArgs& arguments)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->notify(sender, arguments);
		}
	}

	DelegateHandle add(const TDelegate& delegate)
	{
		DelegatePtr pDelegate(static_cast<TDelegate*>(delegate.clone()));
		_delegates.push_back(pDelegate);
		return pDelegate.get();
	}

	void remove(const TDelegate& delegate)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (delegate.equals(**it))
			{
				(*it)->disable();
				_delegates.erase(it);
				return;
			}
		}
	}

	void remove(DelegateHandle delegateHandle)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (*it == delegateHandle)
			{
				(*it)->disable();
				_delegates.erase(it);
				return;
			}
		}
	}

	DefaultStrategy& operator = (const DefaultStrategy& s)
	{
		if (this != &s)
		{
			_delegates = s._delegates;
		}
		return *this;
	}

	void clear()
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->disable();
		}
		_delegates.clear();
	}

	bool empty() const
	{
		return _delegates.empty();
	}

protected:
	Delegates _delegates;
};


template <class TDelegate>
class DefaultStrategy<void,TDelegate>: public NotificationStrategy<void, TDelegate>
	/// Default notification strategy.
	///
	/// Internally, a std::vector<> is used to store
	/// delegate objects. Delegates are invoked in the
	/// order in which they have been registered.
{
public:
	using DelegateHandle = TDelegate*;
	using DelegatePtr = SharedPtr<TDelegate>;
	using Delegates = std::vector<DelegatePtr>;
	using Iterator = typename Delegates::iterator;

public:
	DefaultStrategy() = default;

	DefaultStrategy(const DefaultStrategy& s):
		_delegates(s._delegates)
	{
	}

	DefaultStrategy(DefaultStrategy&& s):
		_delegates(std::move(s._delegates))
	{
	}

	~DefaultStrategy() = default;

	void notify(const void* sender)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->notify(sender);
		}
	}

	DelegateHandle add(const TDelegate& delegate)
	{
		DelegatePtr pDelegate(static_cast<TDelegate*>(delegate.clone()));
		_delegates.push_back(pDelegate);
		return pDelegate.get();
	}

	void remove(const TDelegate& delegate)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (delegate.equals(**it))
			{
				(*it)->disable();
				_delegates.erase(it);
				return;
			}
		}
	}

	void remove(DelegateHandle delegateHandle)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (*it == delegateHandle)
			{
				(*it)->disable();
				_delegates.erase(it);
				return;
			}
		}
	}

	DefaultStrategy& operator = (const DefaultStrategy& s)
	{
		if (this != &s)
		{
			_delegates = s._delegates;
		}
		return *this;
	}

	DefaultStrategy& operator = (DefaultStrategy&& s)
	{
		if (this != &s)
		{
			_delegates = std::move(s._delegates);
		}
		return *this;
	}

	void clear()
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->disable();
		}
		_delegates.clear();
	}

	bool empty() const
	{
		return _delegates.empty();
	}

protected:
	Delegates _delegates;
};


} // namespace Poco


#endif // Foundation_DefaultStrategy_INCLUDED
