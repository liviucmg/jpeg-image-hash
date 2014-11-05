#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "md5.h"

extern "C" {
    #include <jpeglib.h>
}

using namespace std;

void compute_and_print_md5(const char* file_name) {
    const int buffer_lines = 16;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY pJpegBuffer;
    MD5 md5 = MD5();

    // Open file.
    FILE *infile = fopen(file_name, "rb");
    if (infile == NULL) {
        cerr << "Can't open file \"" << file_name  << "\"" << endl;
        exit(1);
    }

    // Initialize decompression.
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, buffer_lines);

    // Read JPEG.
    while (cinfo.output_scanline < cinfo.output_height) {
        int read_lines = jpeg_read_scanlines(&cinfo, pJpegBuffer, buffer_lines);

        for (int y = 0; y < read_lines; y++) {
            md5.update(pJpegBuffer[y], cinfo.output_components * cinfo.output_width);
        }
    }

    // Clean-up.
    fclose(infile);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    // Compute MD5.
    md5.finalize();
    cout << md5.hexdigest() << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "You must supply one and only one argument: the image file path." << endl;

        return 1;
    }

    compute_and_print_md5(argv[1]);

    return 0;
}
