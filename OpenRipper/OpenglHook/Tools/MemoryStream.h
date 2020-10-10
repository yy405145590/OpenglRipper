#pragma once
#include <string>

template <size_t defaultSize = 256>
class MemoryStream
{
public:
	MemoryStream(const std::string &fileName)
		:MemoryStream()
	{
		
		FILE *fp = fopen(fileName.c_str(), "rb");
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			unsigned int fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			if (fileSize > 0)
			{
				if (fileSize > defaultSize)
				{
					m_pBuffer = new char[fileSize];
				}				
				else
				{
					m_pBuffer = m_localBuff;
				}
				m_bufferSize = fileSize;
				fread(m_pBuffer, fileSize, 1, fp);
				m_writePos = fileSize;
			}
			fclose(fp);
		}
	}
	MemoryStream()
	{
		m_pBuffer = m_localBuff;
		m_bufferSize = defaultSize;
		m_readPos = 0;
		m_writePos = 0;
	}
	~MemoryStream(void)
	{
		if (m_pBuffer != m_localBuff)
		{
			delete[] m_pBuffer;
		}
		m_pBuffer = NULL;
		m_bufferSize	= 0;
		m_readPos		= 0;
		m_writePos		= 0;
	}
	bool IsValid(){return m_writePos > 0;}
	template<typename T>
	MemoryStream& operator>> (T& val)
	{
		if (m_readPos + sizeof(T) > m_writePos)
		{
			throw "buffer error";
		}
		else
		{
			_r((uint8 *)&val, sizeof(T));
		}
		return *this;
	}
	template<typename T>
	MemoryStream& operator<< (T& val)
	{
		_w((uint8 *)&val, sizeof(T));
		return *this;
	}
	template<>
	MemoryStream& operator<< (std::string& val)
	{
		UINT32 length = (uint32)val.length();
		*this << length;
		_w((uint8 *)val.c_str(), length);
		return *this;
	}
	template<>
	MemoryStream& operator>> (std::string& val)
	{
		UINT32 length = 0;
		*this >> length;
		char *tmp = new char[length];
		_r((uint8*)tmp, length);
		val.append(tmp, length);
		return *this;
	}
	bool CanRead(unsigned int size)
	{
		return m_readPos + size <= m_writePos;
	}

	void Skip(unsigned int size)
	{
		if (m_readPos + size <= m_writePos)
		{
			m_readPos += size;
		}
	}

	

	void _Resize(uint32 size)
	{
		char *pTmp		= m_pBuffer;
		char tmpSize	= m_bufferSize;
		uint32 newSize = size + m_bufferSize - size % m_bufferSize;
		if (m_bufferSize < newSize)
		{
			if (newSize > defaultSize)
			{
				m_pBuffer		= new char[newSize];
				m_bufferSize	= newSize;
			}
			else
			{
				m_bufferSize	= defaultSize;
				m_pBuffer		= m_localBuff;
			}
			if (pTmp && pTmp != m_pBuffer)
			{
				memcpy(m_pBuffer, pTmp, tmpSize);				
			}
			if (pTmp != m_localBuff)
			{
				delete[] pTmp;
				pTmp = NULL;
			}
		}
	}

private:
	void _r(uint8* p, uint32 size)
	{
		if (m_readPos + size > m_writePos)
		{
			throw "buffer error";
		}
		memcpy(p, m_pBuffer + m_readPos, size);
		m_readPos += size;
	}
	void _w(uint8 *p, uint32 size)
	{
		if (m_writePos + size > m_bufferSize)
		{
			_Resize(m_writePos + size);
		}
		memcpy(m_pBuffer + m_writePos, p, size);
		m_writePos += size;
	}
	


private:
	char *			m_pBuffer;
	unsigned int	m_bufferSize;
	unsigned int	m_readPos;
	unsigned int	m_writePos;

	char			m_localBuff[defaultSize];
};

using	MStream = MemoryStream<>;

