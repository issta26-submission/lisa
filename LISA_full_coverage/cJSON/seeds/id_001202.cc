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
//<ID> 1202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *sref1 = cJSON_CreateStringReference("one");
    cJSON *sref2 = cJSON_CreateStringReference("two");
    cJSON *sref3 = cJSON_CreateStringReference("three");
    cJSON_InsertItemInArray(arr, 0, sref1);
    cJSON_InsertItemInArray(arr, 1, sref2);
    cJSON_InsertItemInArray(arr, 2, sref3);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(arr);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    const char *val = cJSON_GetStringValue(second);
    cJSON *dup_ref = cJSON_CreateStringReference(val);
    cJSON_InsertItemInArray(arr, 1, dup_ref);
    int size_after = cJSON_GetArraySize(arr);

    // step 3: Operate
    char *pretty = cJSON_Print(arr);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(arr, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    (void)size_before;
    (void)size_after;
    cJSON_free(pretty);
    cJSON_free(buf);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}