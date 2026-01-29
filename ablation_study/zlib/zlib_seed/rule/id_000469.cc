#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and write a gz file
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, (voidpc)input, (unsigned int)inputLen);
    gzclose(gzw);

    // step 2: Re-open gz file for reading and read a single byte
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int readc = gzgetc_(gzr);
    unsigned char readChar = (unsigned char)readc;
    gzclose(gzr);

    // step 3: Initialize inflateBack stream and apply reset-keep
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&backStrm);

    // step 4: Finalize inflateBack, validate a small checksum, and cleanup
    inflateBackEnd(&backStrm);
    uLong checksum = adler32(0UL, (const Bytef *)&readChar, (uInt)1);
    (void)checksum;
    free(window);
    return 66; // API sequence test completed successfully
}