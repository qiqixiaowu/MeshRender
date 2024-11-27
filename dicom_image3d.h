#pragma once

#include <string>
#include <vector>

#include "dicom_image.h"

class UDicomImage3D
{
public:
	UDicomImage3D();
	virtual ~UDicomImage3D() {};

	// Volume Size
	void GetDimension(int* dim);

	// Get Volume Spacing
	void GetSpacing(double* Spacing);

	// PixelPointer
	Uint16* GetPixelPointer() const;

	unsigned char* GetPixel() const;

private:

public:
	double m_dSpacing[3];
	double m_dSlope;
	double m_dIntercept;
	unsigned int m_iDimension[3];

	unsigned char* m_spPixel;
	Uint16* m_spPixelPointer;
};
