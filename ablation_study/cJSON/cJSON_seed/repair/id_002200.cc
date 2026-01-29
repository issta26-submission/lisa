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
//<ID> 2200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *floatArr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floatArray", floatArr);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddNumberToObject(meta, "count", 2.0);

    // step 2: Configure
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");
    cJSON *retr_flag = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool is_false_flag = cJSON_IsFalse(retr_flag);
    cJSON *flagnum = cJSON_CreateNumber((double)is_false_flag);
    cJSON_AddItemToObject(meta, "flagValue", flagnum);
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(floatArr, 1, newnum);

    // step 3: Operate and Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "floatArray");
    cJSON *elt = cJSON_GetArrayItem(retrieved, 2);
    double val = cJSON_GetNumberValue(elt);
    cJSON *added = cJSON_CreateNumber(val + 1.0);
    cJSON_AddItemToArray(items, added);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}