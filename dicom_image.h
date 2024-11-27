#pragma once

#include "dcmtk/dcmdata/dcdatset.h" 
 
class UDicomImage
{
public:
	UDicomImage();

	virtual ~UDicomImage() {};

public:
	Uint16* GetPixelPointer() const;

	bool GetPixelValue(const int& iX, const int& iY, double* dValue);
	int GetRow() const
	{
		return m_iRow;
	};
	int GetColunm() const
	{
		return m_iColunm;
	}

	double GetSlop() const
	{
		return m_dSlope;
	}

	double GetIntercept() const
	{
		return m_dIntercept;
	}

	int GetBitAllocated() const
	{
		return m_iBitAllocated;
	}

	void InitImage(DcmDataset& dcmDataset);

private:
	int m_iBitAllocated;
	double m_dSpacing[2];
	int m_iRow;
	int m_iColunm;
	double m_dSlope;
	double m_dIntercept;
	double m_dWW;
	double m_dWC;
	Uint16* m_pPixelPointer;
	size_t m_dataLength;
};