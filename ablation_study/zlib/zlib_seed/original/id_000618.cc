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
//<ID> 618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_data[128];
    Bytef crc_data_block1[32]; // For crc32_combine64
    Bytef crc_data_block2[16]; // For crc32_combine64
    Bytef gz_write_buffer[64];
    char gz_read_buffer[64]; // gzgets reads into char*
    const char* gz_filename = "zlib_test_file_64.gz";
    gzFile gz_file_write = Z_NULL;
    gzFile gz_file_read = Z_NULL;
    uLong adler_val;
    uLong crc_val_part1, crc_val_part2, crc_combined_val;
    off64_t len_block1 = sizeof(crc_data_block1);
    off64_t len_block2 = sizeof(crc_data_block2);
    int ret; // For storing return values

    // Step 2: Setup - Initialize streams and data
    // Initialize deflate stream (even if not used for actual deflate, for deflateResetKeep)
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));

    // Initialize inflate stream
    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    ret = inflateInit_(&inf_strm, ZLIB_VERSION, (int)sizeof(z_stream));

    // Prepare some data buffers
    memset(input_data, 'A', sizeof(input_data));
    memset(crc_data_block1, 'B', sizeof(crc_data_block1));
    memset(crc_data_block2, 'C', sizeof(crc_data_block2));
    
    // Prepare string for gzputs
    memcpy(gz_write_buffer, "This is a test string written by gzputs for zlib 64-bit API.\n", 60);
    gz_write_buffer[60] = '\0'; // Null-terminate the string
    memset(gz_read_buffer, 0, sizeof(gz_read_buffer)); // Clear read buffer

    // Initialize checksums
    adler_val = adler32(0L, Z_NULL, 0); // Initial adler value
    crc_val_part1 = crc32(0L, Z_NULL, 0); // Initial crc value
    crc_val_part2 = crc32(0L, Z_NULL, 0); // Initial crc value

    // Step 3: GZFile Operations (Write) and Checksum Calculation
    gz_file_write = gzopen64(gz_filename, "wb64");
    ret = gzbuffer(gz_file_write, 8192); // Set buffer size for writing
    ret = gzputs(gz_file_write, (const char*)gz_write_buffer); // Write the string to the gzipped file
    ret = gzclose_w(gz_file_write); // Close the write file

    // Calculate adler32 on a portion of input_data
    adler_val = adler32(adler_val, input_data, (uInt)len_block1);

    // Calculate two crc32 values for later combination
    crc_val_part1 = crc32(crc_val_part1, crc_data_block1, (uInt)len_block1);
    crc_val_part2 = crc32(crc_val_part2, crc_data_block2, (uInt)len_block2);

    // Step 4: GZFile Operations (Read) and CRC Combination
    gz_file_read = gzopen64(gz_filename, "rb64");
    ret = gzbuffer(gz_file_read, 4096); // Set a different buffer size for reading
    char* read_result = gzgets(gz_file_read, gz_read_buffer, (int)sizeof(gz_read_buffer)); // Read from the gzipped file

    // Combine the two CRC values using crc32_combine64
    crc_combined_val = crc32_combine64(crc_val_part1, crc_val_part2, len_block2);

    // Step 5: Stream Operations & Edge Cases
    ret = deflateResetKeep(&def_strm); // Reset deflate stream, preserving internal state and allocated memory

    ret = inflateUndermine(&inf_strm, 1); // Undermine the inflate stream by 1 byte

    // Edge Case 1: adler32 with Z_NULL buffer and non-zero length.
    // This should return the 'adler' value unchanged, or the initial value if 'buf' is Z_NULL.
    uLong adler_edge_case_null_buf = adler32(adler_val, Z_NULL, 10);

    // Edge Case 2: gzgets with a buffer too small to hold even a single character.
    // This tests how gzgets handles extremely small buffers. It will likely return NULL or an empty string.
    char tiny_read_buf[1];
    memset(tiny_read_buf, 0, sizeof(tiny_read_buf));
    char* tiny_read_result = gzgets(gz_file_read, tiny_read_buf, (int)sizeof(tiny_read_buf));

    // Edge Case 3: inflateUndermine with a negative value.
    // Zlib documentation implies positive values for 'count'. A negative value might be treated as 0 or lead to an error.
    ret = inflateUndermine(&inf_strm, -5);

    // Step 6: Cleanup
    ret = inflateEnd(&inf_strm); // End the inflate stream, deallocating its state
    ret = deflateEnd(&def_strm); // End the deflate stream, deallocating its state
    ret = gzclose_r(gz_file_read); // Close the read file

    // Remove the temporary gzipped file
    remove(gz_filename);

    printf("API sequence test completed successfully\n");

    return 66;
}