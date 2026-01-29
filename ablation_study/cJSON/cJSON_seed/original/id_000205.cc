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
//<ID> 205
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON *childObj = cJSON_CreateObject();
    cJSON *nestedNum = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(childObj, "n", nestedNum);
    cJSON_AddItemToArray(arr, childObj);
    cJSON *extra = cJSON_CreateObject();

    // step 2: Configure
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(arr, str);
    cJSON_AddItemToObject(extra, "moved", detached);
    int size = cJSON_GetArraySize(arr);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    const char *moved_str = cJSON_GetStringValue(cJSON_GetObjectItem(extra, "moved"));
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    int summary = size + (int)first_val + (moved_str ? 0 : 1) + (int)equal + root->type + dup->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(extra);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}