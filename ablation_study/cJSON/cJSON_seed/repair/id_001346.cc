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
//<ID> 1346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", array);
    int nums[4] = {10, 20, 30, 40};
    cJSON *intArray = cJSON_CreateIntArray(nums, 4);
    cJSON_AddItemToArray(array, intArray);
    cJSON *strRef = cJSON_CreateStringReference("hello");
    cJSON_AddItemToArray(array, strRef);
    cJSON *falseItem = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, falseItem);

    // step 2: Configure
    cJSON_AddStringToObject(root, "note", "int_array_test");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(array, intArray);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    int size_after = cJSON_GetArraySize(array);
    double first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(detached, 0));
    const char *strval = cJSON_GetStringValue(cJSON_GetArrayItem(array, 0));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size_after % 10));
    ((char *)scratch)[2] = strval ? strval[0] : 0;
    ((char *)scratch)[3] = (char)('0' + (((int)first_num) % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}