#include "dicom_image.h"

#include<vector>

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcelem.h"
#include <dcmtk/dcmdata/dcfilefo.h>

UDicomImage::UDicomImage() :
	m_iRow(512),
	m_iColunm(512),
	m_dSlope(1.0),
	m_pPixelPointer(nullptr),
	m_dIntercept(0.0),
	m_dSpacing{ 1.0, 1.0 }
{
}

Uint16* UDicomImage::GetPixelPointer() const
{
	return m_pPixelPointer;
}

bool UDicomImage::GetPixelValue(const int& iX, const int& iY, double* dValue)
{
	return false;
}

void UDicomImage::InitImage(DcmDataset& spDcmDataset)
{
	unsigned short iRow(512), iCol(512);
	if (spDcmDataset.findAndGetUint16(DCM_Rows, iRow).good() &&
		spDcmDataset.findAndGetUint16(DCM_Columns, iCol).good())
	{
		m_iRow = iRow;
		m_iColunm = iCol;
	}

	DcmElement* pElement = nullptr;
	OFCondition result = spDcmDataset.findAndGetElement(DCM_PixelData, pElement);
	if (result.good() && pElement != NULL)
	{
		unsigned short iBitAllocated;
		if (spDcmDataset.findAndGetUint16(DCM_BitsAllocated, iBitAllocated).good())
		{
			m_iBitAllocated = (int)iBitAllocated;
		}

		m_dataLength = pElement->getLength() / sizeof(Uint16);
		m_pPixelPointer = new Uint16[m_dataLength];
		Uint16* image_data = nullptr;
		result = pElement->getUint16Array(image_data);
		if (result.good())
		{
			std::memcpy(m_pPixelPointer, image_data, m_dataLength * sizeof(Uint16));
		}

		std::string spliter("\\");
		std::vector<std::string> splitResult;
		auto splitFun = [&splitResult, spliter](std::string_view strv) {
			size_t start = 0, end = 0;

			while ((end = strv.find(spliter, start)) != std::string_view::npos)
			{
				if (end != start)
				{
					splitResult.push_back(std::string(strv.substr(start, end - start)));
				}
				start = end + spliter.size();
			}
			if (start < strv.size())
			{
				splitResult.push_back(std::string(strv.substr(start)));
			}
		};

		OFString sPixelSpace;
		if (spDcmDataset.findAndGetOFStringArray(DCM_PixelSpacing, sPixelSpace).good())
		{
			splitFun(sPixelSpace.c_str());
			if (2 == splitResult.size())
			{
				m_dSpacing[0] = atof(splitResult[0].c_str());
				m_dSpacing[1] = atof(splitResult[1].c_str());
			}
		}

		bool bExtractedWL = false;
		OFString sWindowWidth, sWindowCenter;
		if ((spDcmDataset.findAndGetOFString(DCM_WindowWidth, sWindowWidth).good()
			&& spDcmDataset.findAndGetOFString(DCM_WindowCenter, sWindowCenter).good()))
		{
			m_dWW = atof(sWindowWidth.c_str());
			m_dWC = atof(sWindowCenter.c_str());
		}

		OFString sSlop;
		if ((spDcmDataset.findAndGetOFString(DCM_RescaleSlope, sSlop).good()))
		{
			m_dSlope = atof(sSlop.c_str());
		}

		OFString sIntercept;
		if (spDcmDataset.findAndGetOFString(DCM_RescaleIntercept, sIntercept).good())
		{
			m_dIntercept = atof(sIntercept.c_str());
		}
	}
}