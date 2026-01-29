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
//<ID> 1349
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *msg = cJSON_CreateString("hello from cJSON");

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw", "{\"embedded\":true}");
    cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "msg", msg);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *count_item = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "size", count_item);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    const char *raw_val = cJSON_GetStringValue(raw_item);
    cJSON *raw_copy = cJSON_CreateString(raw_val);
    cJSON_AddItemToObject(root, "raw_copy", raw_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}