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
//<ID> 2090
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(arr, 0, s2);
    cJSON_InsertItemInArray(arr, 1, s1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "title", "example");

    // step 2: Configure
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 3: Operate and Validate
    cJSON *snapshot = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);
    cJSON *suffix = cJSON_CreateString("gamma");
    cJSON_InsertItemInArray(arr, 2, suffix);
    int arr_size = cJSON_GetArraySize(arr);
    buffer[0] = (char)('0' + (arr_size % 10));

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}