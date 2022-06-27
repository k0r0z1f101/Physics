
//////////////////////////////////////////////////////////////////////////
// Iterator
template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >::StaticRingBufferIterator() :
	m_pOwner		( nullptr ),
	m_uCurrentIdx	( 0 )
{}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >::StaticRingBufferIterator(	StaticRingBufferIterator< Type, uSize > const& oStaticRingBufferIterator ) :
	m_pOwner		( oStaticRingBufferIterator.m_pOwner ),
	m_uCurrentIdx	( oStaticRingBufferIterator.m_uCurrentIdx )
{}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >::StaticRingBufferIterator(	StaticRingBuffer< Type, uSize >*							pOwner,
																	typename	StaticRingBuffer< Type, uSize >::SizeType const	uCurrent ) :
	m_pOwner		( pOwner ),
	m_uCurrentIdx	( uCurrent )
{}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >&	StaticRingBufferIterator< Type, uSize >::operator=(	StaticRingBufferIterator< Type, uSize > const& oStaticRingBufferIterator )
{
	m_pOwner		= oStaticRingBufferIterator.m_pOwner;
	m_uCurrentIdx	= oStaticRingBufferIterator.m_uCurrentIdx;

	return *this;
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >	StaticRingBufferIterator< Type, uSize >::Previous()
{
	if ( m_pOwner && m_pOwner->m_uSize > 0 )
	{
		StaticRingBufferIterator< Type, uSize > oCopy( *this );

		--oCopy;
		if ( oCopy )
		{
			return oCopy;
		}
		else
		{
			return StaticRingBufferIterator< Type, uSize >( m_pOwner, ( m_pOwner->m_uBegin + m_pOwner->m_uSize - 1 )%uSize );
		}
	}
	else
	{
		return StaticRingBufferIterator< Type, uSize >();
	}
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >	StaticRingBufferIterator< Type, uSize >::Next()
{
	if ( m_pOwner && m_pOwner->m_uSize > 0 )
	{
		StaticRingBufferIterator< Type, uSize > oCopy( *this );

		++oCopy;
		if ( oCopy )
		{
			return oCopy;
		}
		else
		{
			return StaticRingBufferIterator< Type, uSize >( m_pOwner, ( m_uCurrentIdx + 1 )%uSize );
		}
	}
	else
	{
		return StaticRingBufferIterator< Type, uSize >();
	}
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >&	StaticRingBufferIterator< Type, uSize >::operator++()
{
	if ( m_uCurrentIdx == ( m_pOwner->m_uBegin + m_pOwner->m_uSize - 1 )%uSize )
	{
		m_pOwner		= nullptr;
		m_uCurrentIdx	= 0;
	}
	else
	{
		++m_uCurrentIdx;
		m_uCurrentIdx	%= uSize;
	}

	return *this;
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >&	StaticRingBufferIterator< Type, uSize >::operator++(int)
{
	if ( m_uCurrentIdx == ( m_pOwner->m_uBegin + m_pOwner->m_uSize - 1 )%uSize )
	{
		m_pOwner		= nullptr;
		m_uCurrentIdx	= 0;
	}
	else
	{
		++m_uCurrentIdx;
		m_uCurrentIdx	%= uSize;
	}

	return *this;
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >&	StaticRingBufferIterator< Type, uSize >::operator--()
{
	if ( m_uCurrentIdx == m_pOwner->m_uBegin )
	{
		m_pOwner		= nullptr;
		m_uCurrentIdx	= 0;
	}
	else
	{
		if ( m_uCurrentIdx == 0 )
			m_uCurrentIdx = uSize - 1;
		else
			--m_uCurrentIdx;
	}

	return *this;
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >&	StaticRingBufferIterator< Type, uSize >::operator--(int)
{
	if ( m_uCurrentIdx == m_pOwner->m_uBegin )
	{
		m_pOwner		= nullptr;
		m_uCurrentIdx	= 0;
	}
	else
	{
		if ( m_uCurrentIdx == 0 )
			m_uCurrentIdx = uSize - 1;
		else
			--m_uCurrentIdx;
	}

	return *this;
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBufferIterator< Type, uSize >::Reference		StaticRingBufferIterator< Type, uSize >::operator*()
{
	return m_pOwner->m_oDatas[ m_uCurrentIdx ];
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBufferIterator< Type, uSize >::ConstReference	StaticRingBufferIterator< Type, uSize >::operator*() const
{
	return m_pOwner->m_oDatas[ m_uCurrentIdx ];
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBufferIterator< Type, uSize >::Pointer		StaticRingBufferIterator< Type, uSize >::operator->()
{
	return &m_pOwner->m_oDatas[ m_uCurrentIdx ];
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBufferIterator< Type, uSize >::ConstPointer	StaticRingBufferIterator< Type, uSize >::operator->() const
{
	return &m_pOwner->m_oDatas[ m_uCurrentIdx ];
}

template < typename Type, UInt32 uSize >
inline
Bool	StaticRingBufferIterator< Type, uSize >::operator==( StaticRingBufferIterator const& oIterator )
{
	return m_pOwner == oIterator.m_pOwner && m_uCurrentIdx == oIterator.m_uCurrentIdx;
}

template < typename Type, UInt32 uSize >
inline
Bool	StaticRingBufferIterator< Type, uSize >::operator==( StaticRingBufferIterator const& oIterator ) const
{
	return m_pOwner == oIterator.m_pOwner && m_uCurrentIdx == oIterator.m_uCurrentIdx;
}

template < typename Type, UInt32 uSize >
inline
Bool	StaticRingBufferIterator< Type, uSize >::operator!=( StaticRingBufferIterator const& oIterator )
{
	return m_pOwner != oIterator.m_pOwner || m_uCurrentIdx != oIterator.m_uCurrentIdx;
}

template < typename Type, UInt32 uSize >
inline
Bool	StaticRingBufferIterator< Type, uSize >::operator!=( StaticRingBufferIterator const& oIterator ) const
{
	return m_pOwner != oIterator.m_pOwner || m_uCurrentIdx != oIterator.m_uCurrentIdx;
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >::operator Bool()
{
	return m_pOwner != nullptr;
}

template < typename Type, UInt32 uSize >
inline
StaticRingBufferIterator< Type, uSize >::operator Bool() const
{
	return m_pOwner != nullptr;
}

//////////////////////////////////////////////////////////////////////////
template < typename Type, UInt32 uSize >
inline
StaticRingBuffer< Type, uSize >::StaticRingBuffer()	:
	m_uBegin( 0 ),
	m_uEnd	( 0 ),
	m_uSize	( 0 ),
	m_oDatas()
{
	//static_assert( uSize > 0, "RingBuffer need Init Size positive & not null" );
}

template < typename Type, UInt32 uSize >
inline
StaticRingBuffer< Type, uSize >::StaticRingBuffer( StaticRingBuffer< Type, uSize > const& oStaticRingBuffer )	:
	m_uBegin( oStaticRingBuffer.m_uBegin ),
	m_uEnd	( oStaticRingBuffer.m_uEnd ),
	m_uSize	( oStaticRingBuffer.m_uSize ),
	m_oDatas( oStaticRingBuffer.m_oDatas )
{}

template < typename Type, UInt32 uSize >
inline
StaticRingBuffer< Type, uSize >::StaticRingBuffer( StaticRingBuffer< Type, uSize >&& oStaticRingBuffer )	:
	m_uBegin( oStaticRingBuffer.m_uBegin ),
	m_uEnd	( oStaticRingBuffer.m_uEnd ),
	m_uSize	( oStaticRingBuffer.m_uSize ),
	m_oDatas( std::move( oStaticRingBuffer.m_oDatas ) )
{}

template < typename Type, UInt32 uSize >
inline
StaticRingBuffer< Type, uSize >::~StaticRingBuffer()
{}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBuffer< Type, uSize >::Iterator			StaticRingBuffer< Type, uSize >::Begin()
{
	if ( m_uSize == 0 )
		return StaticRingBufferIterator< Type, uSize >( nullptr, 0 );
	else
		return StaticRingBufferIterator< Type, uSize >( this, m_uBegin );
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBuffer< Type, uSize >::ConstIterator	StaticRingBuffer< Type, uSize >::Begin() const
{
	if ( m_uSize == 0 )
		return StaticRingBufferIterator< Type, uSize >( nullptr, 0 );
	else
		return StaticRingBufferIterator< Type, uSize >( this, m_uBegin );
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBuffer< Type, uSize >::Iterator			StaticRingBuffer< Type, uSize >::End()
{
	return StaticRingBufferIterator< Type, uSize >( nullptr, 0 );
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBuffer< Type, uSize >::ConstIterator	StaticRingBuffer< Type, uSize >::End() const
{
	return StaticRingBufferIterator< Type, uSize >( nullptr, 0 );
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBuffer< Type, uSize >::SizeType	StaticRingBuffer< Type, uSize >::Size() const
{
	return m_uSize;
}

template < typename Type, UInt32 uSize >
inline
typename StaticRingBuffer< Type, uSize >::SizeType	StaticRingBuffer< Type, uSize >::Capacity() const
{
	return uSize;
}

template < typename Type, UInt32 uSize >
inline
void				StaticRingBuffer< Type, uSize >::PushFront( Type const& oValue )
{
	if ( m_uBegin == 0 )
		m_uBegin = uSize - 1;
	else
		--m_uBegin;

	if ( m_uSize == uSize )
	{
		( &m_oDatas[ m_uBegin ] )->~Type();
		m_oDatas[ m_uBegin ]	=	oValue;
		if ( m_uEnd == 0 )
			m_uEnd = uSize - 1;
		else
			--m_uEnd;
	}
	else
	{
		++m_uSize;
		m_oDatas[ m_uBegin ]	=	oValue;
	}
}

template < typename Type, UInt32 uSize >
inline
void				StaticRingBuffer< Type, uSize >::PushBack( Type const& oValue )
{
	if ( m_uSize == uSize )
	{
		( &m_oDatas[ m_uBegin ] )->~Type();
		++m_uBegin;
		m_uBegin	%=	uSize;
	}
	m_oDatas[ m_uEnd ]	=	oValue;
	++m_uEnd;
	m_uEnd		%=	uSize;
	m_uSize		=	( m_uSize == uSize ) ? uSize : ( m_uSize + 1 );
}

template < typename Type, UInt32 uSize >
inline
void				StaticRingBuffer< Type, uSize >::PopFront()
{
	if ( m_uSize > 0 )
	{
		( &m_oDatas[ m_uBegin ] )->~Type();
		++m_uBegin;
		m_uBegin	%= uSize;
		--m_uSize;
	}
}

template < typename Type, UInt32 uSize >
inline
void				StaticRingBuffer< Type, uSize >::PopBack()
{
	if ( m_uSize > 0 )
	{
		( &m_oDatas[ m_uEnd ] )->~Type();
		if ( m_uEnd == 0 )
			m_uEnd = uSize - 1;
		else
			--m_uEnd;
		--m_uSize;
	}
}

template < typename Type, UInt32 uSize >
inline
void				StaticRingBuffer< Type, uSize >::Clear()
{
	while ( m_uSize > 0 )
	{
		PopFront();
	}
}

// Implement the same algo as std::list
template < typename Type, UInt32 uSize >
inline
void				StaticRingBuffer< Type, uSize >::Sort()
{
	std::sort( &m_oDatas[ 0 ], &m_oDatas[ uSize - 1 ] );
}

template < typename Type, UInt32 uSize >
template < class Comparator >
inline
void				StaticRingBuffer< Type, uSize >::Sort( Comparator oComp )
{
	std::sort( &m_oDatas[ 0 ], &m_oDatas[ uSize - 1 ], oComp );
}
