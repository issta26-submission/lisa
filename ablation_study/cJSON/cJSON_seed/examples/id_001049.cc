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
//<ID> 1049
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"nums\":[1,2,3],\"label\":\"original\"}";
    size_t len1 = sizeof(json1) - 1;
    const char *end1 = NULL;
    cJSON *root1 = cJSON_ParseWithLengthOpts(json1, len1, &end1, 1);
    const char json2[] = "{\"status\":\"ok\",\"count\":0}";
    cJSON *root2 = cJSON_Parse(json2);
    cJSON *extraArr = cJSON_CreateArray();
    cJSON *n4 = cJSON_CreateNumber(4.0);
    cJSON *n5 = cJSON_CreateNumber(5.0);
    cJSON *ref = cJSON_CreateStringReference("ref-label");

    // step 2: Configure
    cJSON_AddItemToArray(extraArr, n4);
    cJSON_AddItemToArray(extraArr, n5);
    cJSON_AddItemToObject(root2, "extra", extraArr);
    cJSON_AddItemToObject(root2, "ref", ref);

    // step 3: Operate & Validate
    cJSON *nums = cJSON_GetObjectItem(root1, "nums");
    int count = cJSON_GetArraySize(nums);
    cJSON *item0 = cJSON_GetArrayItem(nums, 0);
    cJSON *item1 = cJSON_GetArrayItem(nums, 1);
    cJSON *item2 = cJSON_GetArrayItem(nums, 2);
    double sum = cJSON_GetNumberValue(item0) + cJSON_GetNumberValue(item1) + cJSON_GetNumberValue(item2);
    cJSON *sumNum = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root1, "sum", sumNum);
    cJSON *countNum = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root2, "original_count", countNum);

    // step 4: Cleanup
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    return 66; // API sequence test completed successfully
}