#include "dicom_image3d.h"

UDicomImage3D::UDicomImage3D() :
	m_spPixelPointer(nullptr),
	m_dSlope(0.0),
	m_dIntercept(0.0),
	m_iDimension{ 1, 1, 1 },
	m_dSpacing{ 1.0, 1.0,1.0 }
{
}

void UDicomImage3D::GetDimension(int* dim)
{
	std::memcpy(dim, m_iDimension, 3 * sizeof(int));
}

void UDicomImage3D::GetSpacing(double* dSpacing)
{
	std::memcpy(dSpacing, m_dSpacing, 3 * sizeof(double));
}

Uint16* UDicomImage3D::GetPixelPointer() const
{
	return m_spPixelPointer;
}

unsigned char* UDicomImage3D::GetPixel() const
{
	return m_spPixel;
}
