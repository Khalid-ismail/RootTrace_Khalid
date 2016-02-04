#include <cv.h>
#include <zbar.h>
#include <iostream>
#include <iomanip>

#include "../Header_Files/stdafx.h"
class CBarcode
{
public:
	static __int64 findBarcodeInImage (IplImage* m_image);

};