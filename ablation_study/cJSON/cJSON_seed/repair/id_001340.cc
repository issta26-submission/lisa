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
//<ID> 1340
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
    int nums[4] = {10, 20, 30, 40};
    cJSON *intarr = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToObject(root, "ints", intarr);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *sref = cJSON_CreateStringReference("reference_text");
    cJSON_AddItemToObject(root, "ref", sref);
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, f);
    cJSON *n = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, n);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, first);
    int size_after = cJSON_GetArraySize(arr);
    int intcount = cJSON_GetArraySize(intarr);
    const char *sval = cJSON_GetStringValue(sref);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = sval ? sval[0] : 0;
    ((char *)scratch)[2] = (char)('0' + (size_before % 10));
    ((char *)scratch)[3] = (char)('0' + (size_after % 10));
    ((char *)scratch)[4] = (char)('0' + (intcount % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}