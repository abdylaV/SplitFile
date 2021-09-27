// SplitFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 5)
	{
		std::cout << "----------------------------------------------\n";
		//std::cout << "<For quit from application please press ESC>\n";
		std::cout << "SplitFile.exe <InputFile> <OutputFile> <OffSet> <Size>" << std::endl;
		std::cout << "----------------------------------------------\n";

		return 0;
	}

	std::wstring filenameIn = argv[1];
	std::wstring filenameOut = argv[2];

	uint64_t uiOffSet = _wtoi(argv[3]);
	uint64_t uiSizeSplit = _wtoi(argv[4]);

	///////////////////
	// for test
	///////////////////
	//std::wstring filenameIn = L"D:\\test.file";
	//std::wstring filenameOut = L"D:\\test2.file";
	//
	//uint64_t uiOffSet = 0;
	//uint64_t uiSizeSplit = 50;
	///////////////////

	std::ifstream ifs;
	ifs.open(filenameIn.c_str(), std::ifstream::in | std::ifstream::binary);

	if (ifs.good())
	{
		std::ofstream ofs;
		ofs.open(filenameOut.c_str(), std::ifstream::out | std::ifstream::binary);

		if (ofs.good())
		{
			// get pointer to associated buffer object
			std::filebuf* pbuf = ifs.rdbuf();

			// get file size using buffer's members
			uint64_t uiSizeFile = pbuf->pubseekoff(0, ifs.end, ifs.in);

			if ((uiOffSet >= 0) && (uiOffSet < uiSizeFile) && ((uiOffSet + uiSizeSplit) < uiSizeFile))
			{
				const size_t iSizeData = 1024 * 1024;
				uint64_t uiBytes = iSizeData;
				float fProcess = 0.f;

				std::vector<char> pData;

				try
				{
					pData.resize(iSizeData);

					ifs.seekg(uiOffSet);

					for (uint64_t i = 0; i < uiSizeSplit; i += uiBytes)
					{
						if ((uiSizeSplit - i) < 1024)
							uiBytes = (uiSizeSplit - i);

						ifs.read(pData.data(), uiBytes);
						ofs.write(pData.data(), uiBytes);

						fProcess = (static_cast<float>(i) / static_cast<float>(uiSizeSplit)) * 100.f;
						std::cout << "\rprocess:                             ";
						std::cout << "\rprocess: " << fProcess << "%";
					}
					std::cout << "\rprocess:                             ";
					std::cout << "\rprocess: " << 100.f << "%" << std::endl;
				} 
				catch (std::exception &e)
				{
					std::cout << "Exception: " << e.what() << std::endl;
				}
			}
			else
			{
				std::cout << "Input params (offest or size of split) incorrect!" << std::endl;
			}

			ofs.close();
		}
		else
		{
			std::cout << "Can not create output file!" << std::endl;
		}

		ifs.close();
	}
	else
	{
		std::cout << "Can not open input file!" << std::endl;
	}

    return 0;
}
