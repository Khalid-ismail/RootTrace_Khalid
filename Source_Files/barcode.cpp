#include "stdafx.h"
#include "barcode.h"

#define STR(s) #s

using namespace std;
using namespace zbar;
using namespace System::Windows::Forms;

__int64 CBarcode::findBarcodeInImage (IplImage* m_image)
{
    

    // create a reader
    ImageScanner scanner;

    // configure the reader
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	//scanner.set_config(ZBAR_CODE128 , ZBAR_CFG_ADD_CHECK, 1);
	//scanner.set_config(ZBAR_CODE128 , ZBAR_CFG_EMIT_CHECK, 1);

    // obtain image data

	IplImage* grey  = cvCreateImage(cvGetSize(m_image), 8, 1);
	cvCvtColor(m_image, grey, CV_RGB2GRAY);

   
    const void *raw = grey->imageData;

    // wrap image data
    Image image(grey->width, grey->height, "Y800", raw, grey->width * grey->height);

    // scan the image for barcodes
    int n = scanner.scan(image);

	//MessageBox::Show("Scanned");
	System::String ^stype;
	System::String ^s;
    // extract results
    for(Image::SymbolIterator symbol = image.symbol_begin();
        symbol != image.symbol_end();
        ++symbol) {
			// do something useful with results
			//MessageBox::Show("decoded " + symbol->get_type_name());
			stype =gcnew System::String(symbol->get_type_name().c_str());
			s =gcnew System::String(symbol->get_data().c_str());
			MessageBox::Show("Type: "+stype+"\nCode: "+s);
			//cout << "test"<<endl;
    }

	__int64 barcodeNumber = System::Int64::Parse(s);
    // clean up
    image.set_data(NULL, 0);
	cvReleaseImage(&grey);

    return barcodeNumber;
}
