#pragma once

#include "Common/Kernel.h"

#include <array>
#include <algorithm>

namespace SK
{
	template < typename Type, UInt32 uSize >
	class StaticRingBuffer;

	template < typename Type, UInt32 uSize >
	class StaticRingBufferIterator
	{
	public:
		typedef	Type*						Iterator;
		typedef	Type const*					ConstIterator;
		typedef	Type*						Pointer;
		typedef	Type const*					ConstPointer;
		typedef	Type&						Reference;
		typedef	Type const&					ConstReference;

		StaticRingBufferIterator();
		StaticRingBufferIterator( StaticRingBufferIterator const& oStaticRingBufferIterator );
		StaticRingBufferIterator( StaticRingBuffer< Type, uSize >* pOwner, typename StaticRingBuffer< Type, uSize >::SizeType const uCurrent );
		StaticRingBufferIterator&	operator=( StaticRingBufferIterator const& oStaticRingBufferIterator );

		StaticRingBufferIterator	Previous();
		StaticRingBufferIterator	Next();

		StaticRingBufferIterator&	operator++();
		StaticRingBufferIterator&	operator++(int);
		StaticRingBufferIterator&	operator--();
		StaticRingBufferIterator&	operator--(int);

		Reference		operator*();
		ConstReference	operator*() const;
		Pointer			operator->();
		ConstPointer	operator->() const;

		Bool	operator==( StaticRingBufferIterator const& oIterator );
		Bool	operator==( StaticRingBufferIterator const& oIterator ) const;
		Bool	operator!=( StaticRingBufferIterator const& oIterator );
		Bool	operator!=( StaticRingBufferIterator const& oIterator ) const;
		operator Bool();
		operator Bool() const;

	protected:
		StaticRingBuffer< Type, uSize >*						m_pOwner;
		typename	StaticRingBuffer< Type, uSize >::SizeType	m_uCurrentIdx;
	};

	template < typename Type, UInt32 uSize >
	class StaticRingBuffer
	{
	public:
		typedef	StaticRingBuffer< Type, uSize >				ContainerType;
		typedef	Type										ValueType;
		typedef	UInt32											SizeType;
		typedef StaticRingBufferIterator< Type, uSize >		Iterator;
		typedef StaticRingBufferIterator< Type, uSize >		ConstIterator;
		typedef	Type&										Reference;
		typedef	Type const&									ConstReference;

		StaticRingBuffer();
		StaticRingBuffer( StaticRingBuffer const& oStaticRingBuffer );
		StaticRingBuffer( StaticRingBuffer&& oStaticRingBuffer );
		~StaticRingBuffer();

		Iterator			Begin();
		ConstIterator		Begin() const;
		Iterator			End();
		ConstIterator		End() const;

		Iterator			begin()			{ return Begin(); }
		ConstIterator		begin() const	{ return Begin(); }
		Iterator			end()			{ return End(); }
		ConstIterator		end() const		{ return End(); }

		SizeType			Size() const;
		SizeType			Capacity() const;

		SizeType			size() const	{ return Size(); }

		void				PushFront( Type const& oValue );
		void				PushBack( Type const& oValue );
		void				PopFront();
		void				PopBack();
		void				Clear();

		void				push_back( Type const& oValue )
											{ return PushBack( oValue ); }
		void				pop_back()		{ return PopBack(); }
		void				clear()			{ return Clear(); }

		void				Sort();
		template < class Comparator >
		void				Sort( Comparator oComp );

		friend
		class StaticRingBufferIterator< Type, uSize >;

	private:
		SizeType					m_uBegin;
		SizeType					m_uEnd;
		SizeType					m_uSize;
		std::array< Type, uSize >	m_oDatas;
	};

	#include "StaticRingBuffer.hpp"
}
