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
//<ID> 593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(meta, "enabled", 1);
    cJSON *title = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "title", title);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    int count = cJSON_GetArraySize(numbers);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *enabled_item = cJSON_GetObjectItem(got_meta, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled_item);
    cJSON_AddBoolToObject(root, "meta_enabled_copy", enabled_flag);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *a1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *a2 = cJSON_GetArrayItem(numbers, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "average", average);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}