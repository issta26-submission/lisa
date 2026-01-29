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
//<ID> 1339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToObject(config, "numbers", nums);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *ans = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", ans);

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(nums);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(nums, n1, replacement);
    int size_after = cJSON_GetArraySize(nums);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    double val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(nums, 0));
    double val_answer = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "answer"));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size_before % 10));
    ((char *)scratch)[2] = (char)('0' + (size_after % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced ? 1 : 0));
    ((char *)scratch)[4] = (char)('0' + ((int)val0 % 10));
    ((char *)scratch)[5] = (char)('0' + ((int)val_answer % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}