#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 2094
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s0 = cJSON_CreateString("first");
    cJSON_InsertItemInArray(arr, 0, s0);
    cJSON *s1 = cJSON_CreateString("second");
    cJSON_InsertItemInArray(arr, 1, s1);
    cJSON_AddStringToObject(root, "greeting", "hello world");

    // step 2: Configure and Operate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 3: Validate
    cJSON *got = cJSON_GetObjectItem(root, "greeting");
    const char *gstr = cJSON_GetStringValue(got);
    buffer[0] = printed_ok ? '1' : '0';
    buffer[1] = gstr ? gstr[0] : '\0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}