#pragma once

#include <Poco/AbstractCache.h>
#include "AlefPoolStrategy.h"

using Poco::AbstractCache;
using Poco::FastMutex;
using Poco::Timestamp;

template <
	class TKey,
	class TValue,
	class TMutex = FastMutex,
	class TEventMutex = FastMutex
>
class AlefPool : public AbstractCache<TKey, TValue, AlefPoolStrategy<TKey, TValue>, TMutex, TEventMutex>
{
public:
	AlefPool(Timestamp::TimeDiff expire = 600000) :
		AbstractCache<TKey, TValue, AlefPoolStrategy<TKey, TValue>, TMutex, TEventMutex>(AlefPoolStrategy<TKey, TValue>(expire))
	{
	}

	~AlefPool()
	{
	}

private:
	AlefPool(const AlefPool& aCache);
	AlefPool& operator = (const AlefPool& aCache);
};