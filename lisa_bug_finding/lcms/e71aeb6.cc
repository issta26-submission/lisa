#include <cstddef>
#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cassert>

int test_api_sequence() {
    // step 1: Declarations

    cmsContext ctx = NULL;
    cmsHANDLE it8_handle = NULL;
    const char* it8_data_src = NULL;
    char* buffer_exact = NULL;
    char* buffer_less = NULL;
    cmsUInt32Number bytes_needed = 0;
    cmsUInt32Number bytes_written_exact = 0;
    cmsUInt32Number bytes_written_less = 0;
    size_t len_exact = 0;
    size_t len_less = 0;
    cmsBool save_result = 0;
    ctx = cmsCreateContext(NULL, NULL);
    assert(ctx != NULL);
    // step 2: setup
    cmsUInt32Number cmm_version = cmsGetEncodedCMMversion();
    assert(cmm_version > 0);
    it8_data_src = 
        "IT8.7/1\n"
        "DESCRIPTOR \"D\"\n"
        "ORIGINATOR \"O\"\n"
        "Hello\n";
    assert(it8_data_src != NULL);
    assert(strlen(it8_data_src) > 0);
    it8_handle = cmsIT8LoadFromMem(
        ctx, 
        (void*)it8_data_src, 
        (cmsUInt32Number)(strlen(it8_data_src) + 1)
    );
    assert(it8_handle != NULL);
    assert(cmsIT8TableCount(it8_handle) > 0);
    // step 3: core operation
    save_result = cmsIT8SaveToMem(it8_handle, NULL, &bytes_needed);
    assert(save_result != 0); 
    assert(bytes_needed > 0); 
    assert(bytes_needed > strlen(it8_data_src)); 

    buffer_exact = (char*)malloc(bytes_needed);
    assert(buffer_exact != NULL);
    memset(buffer_exact, 0, bytes_needed);
    assert(buffer_exact[0] == 0);
    assert(buffer_exact[bytes_needed - 1] == 0);

    bytes_written_exact = bytes_needed;
    save_result = cmsIT8SaveToMem(it8_handle, buffer_exact, &bytes_written_exact);
    assert(save_result != 0); 
    len_exact = strlen(buffer_exact);
    assert(len_exact > 0);
    assert(len_exact < bytes_needed);
    assert(buffer_exact[len_exact] == '\0'); 

    buffer_less = (char*)malloc(bytes_needed - 1);
    assert(buffer_less != NULL);
    memset(buffer_less, 0, bytes_needed - 1);

    bytes_written_less = bytes_needed - 1;
    save_result = cmsIT8SaveToMem(it8_handle, buffer_less, &bytes_written_less);
    len_less = strlen(buffer_less);
    assert(len_less < bytes_needed);
    assert(len_less < len_exact);
    // step 4: cleanup
    cmsDeleteContext(ctx);
    assert(ctx != NULL);
    (void)save_result;
    (void)len_less;
    (void)len_exact;
    (void)it8_data_src;
    (void)cmm_version;

    return 66;
}

int main() {
    for (int i = 0; i < 10; i++) {
        int result = test_api_sequence();
        if (result != 66) {
            std::cerr << "API sequence test failed with error code: " << result << std::endl;
            exit(100);
        }
    }
    printf("Test completed successfully.\n");
    return 0;
}
