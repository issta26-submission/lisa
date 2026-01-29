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
//<ID> 1614
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "value", 3.14);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *itm1 = cJSON_GetArrayItem(arr, 1);
    double v = cJSON_GetNumberValue(itm1);
    cJSON *double_item = cJSON_CreateNumber(v * 2.0);
    cJSON_AddItemToArray(arr, double_item);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *meta_val = cJSON_GetObjectItemCaseSensitive(detached_meta, "value");
    double mv = cJSON_GetNumberValue(meta_val);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    int idx0 = ((int)v) % 10;
    int idx1 = ((int)mv) % 10;
    buffer[0] = (char)('0' + idx0);
    buffer[1] = (char)('0' + idx1);
    char *s = cJSON_GetStringValue(s1);
    buffer[2] = s[0];

    // step 4: Cleanup
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}