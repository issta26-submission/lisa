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
//<ID> 1203
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
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *first = cJSON_CreateString("original");
    cJSON_AddItemToArray(arr, first);
    cJSON *ref = cJSON_CreateStringReference("reference-string");
    cJSON_InsertItemInArray(arr, 0, ref);

    // step 2: Configure / Serialize
    char *printed = cJSON_Print(root);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 3: Operate
    int size = cJSON_GetArraySize(arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    char *second_str = cJSON_GetStringValue(second_item);

    // step 4: Validate & Cleanup
    (void)size;
    (void)second_str;
    (void)prealloc_ok;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}