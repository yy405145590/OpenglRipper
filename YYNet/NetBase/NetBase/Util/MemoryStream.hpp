#pragma once

template <size_t InitBufferSize = 8 * 1024>
class MemoryStream
{
public:
	MemoryStream() :
		m_pBuffer(NULL),
		m_bufferSize(0),
		m_rPos(0),
		m_wPos(0)
	{
	}
	~MemoryStream() 
	{
		if (m_pBuffer != NULL && m_pBuffer != m_initBuf)
		{
			delete[] m_pBuffer;
		}
	}
	//禁用拷贝构造和等号赋值
	MemoryStream(MemoryStream &other);
	MemoryStream& operator= (MemoryStream &other);

	void Append(char* pBuf, UINT32 nSize)
	{
		_w(pBuf, nSize);
	}
	void Reset()
	{
		m_rPos = 0;
		m_wPos = 0;
	}
	void GetWriteBuffer(char* &pBuf, UINT32 nSize)
	{
		if (m_bufferSize - m_wPos < nSize)
		{
			Resize(m_wPos + nSize);
		}
		UINT32 nPos = m_wPos;
		m_wPos += nSize;
		pBuf = m_pBuffer + nPos;
	}

	void GetReadBuffer(char* &pBuf, UINT32 &nSize)
	{
		if (m_wPos - m_rPos > 0)
		{
			pBuf = m_pBuffer + m_rPos;
			nSize = m_wPos - m_rPos;
			m_rPos += nSize;
		}
		else
		{
			pBuf = NULL;
			nSize = 0;
		}
	}

	template <typename T>
	MemoryStream& operator<< (T &&val)
	{
		_w((char *)&val, sizeof(T));
		return *this;
	}
	template <typename T>
	MemoryStream& operator>> (T &val)
	{
		_r((char *)&val, sizeof(T));
		return *this;
	}

	template<>
	MemoryStream& operator<< (const std::string& val)
	{
		UINT32 length = (UINT32)val.length();
		*this << length;
		_w((char *)val.c_str(), length);
		return *this;
	}

	template<>
	MemoryStream& operator<< (const std::string&& val)
	{
		UINT32 length = (UINT32)val.length();
		*this << length;
		_w((char *)val.c_str(), length);
		return *this;
	}
	template<>
	MemoryStream& operator<< (std::string& val)
	{
		UINT32 length = (UINT32)val.length();
		*this << length;
		_w((char *)val.c_str(), length);
		return *this;
	}

	template<>
	MemoryStream& operator<< (std::string&& val)
	{
		UINT32 length = (UINT32)val.length();
		*this << length;
		_w((char *)val.c_str(), length);
		return *this;
	}
	template<>
	MemoryStream& operator>> (std::string& val)
	{
		UINT32 length = 0;
		*this >> length;
		char *tmp = new char[length];
		_r((char*)tmp, length);
		val.append(tmp, length);
		return *this;
	}
	template<>
	MemoryStream& operator<< (char* &val)
	{
		std::string s = val;
		*this << s;
		return *this;
	}

	template<>
	MemoryStream& operator<< (char* &&val)
	{
		std::string s = val;
		*this << s;
		return *this;
	}
	template<>
	MemoryStream& operator<< (const char* &val)
	{
		std::string s = val;
		*this << s;
		return *this;
	}

	template<>
	MemoryStream& operator<< (const char* &&val)
	{
		std::string s = val;
		*this << s;
		return *this;
	}
	
	void Resize(UINT32 nSize)		
	{
		char* pNewBuffer = NULL;
		if (nSize <= InitBufferSize)
		{
			pNewBuffer = m_initBuf;
			m_bufferSize = InitBufferSize;
		}
		else
		{
			UINT32 newSize = nSize + InitBufferSize - (nSize & (InitBufferSize - 1));
			pNewBuffer = new char[newSize];
			if (m_pBuffer != NULL)
			{
				memcpy(pNewBuffer, m_pBuffer, m_bufferSize);
				if (m_pBuffer != m_initBuf) 
				{
					delete m_pBuffer;
					m_pBuffer = NULL;
				}
			}
			m_bufferSize = newSize;
		}
		m_pBuffer = pNewBuffer;
	}

private:
	void _w(char *pBuf, UINT32 nSize)
	{
		if (pBuf == NULL)
		{
			return;
		}
		if (m_wPos + nSize > m_bufferSize)
		{
			Resize(m_wPos + nSize);
		}
		memcpy(m_pBuffer + m_wPos, pBuf, nSize);
		m_wPos += nSize;
	}

	void _r(char *pBuf, UINT32 nSize)
	{
		if (pBuf == NULL)
		{
			return;
		}
		if (m_rPos + nSize > m_wPos)
		{
			return;
		}
		memcpy(pBuf, m_pBuffer + m_rPos, nSize);
		m_rPos += nSize;
	}

	char*	m_pBuffer;
	UINT32	m_bufferSize;
	UINT32	m_rPos;
	UINT32	m_wPos;

	char m_initBuf[InitBufferSize];

	static_assert(!(InitBufferSize & (InitBufferSize - 1)), "buffer size is not a power of 2");
};