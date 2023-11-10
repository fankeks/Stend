#include <tiffio.h>
int main()
{
	TIFF* tif = TIFFOpen("foo.tif", "r");
	TIFFClose(tif);
	return 0;
}