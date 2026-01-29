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
//<ID> 1369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *msg = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(child, "values", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "message", msg);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    double v = cJSON_GetNumberValue(n1);
    double newv = v + (double)is_obj;
    cJSON_SetNumberHelper(n1, newv);
    const char *text = cJSON_GetStringValue(msg);
    cJSON *msg_copy = cJSON_CreateString(text);
    cJSON_AddItemToObject(root, "message_copy", msg_copy);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}