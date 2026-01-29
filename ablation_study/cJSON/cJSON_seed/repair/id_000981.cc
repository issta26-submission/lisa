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
//<ID> 981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, second);
    cJSON *third = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, third);
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddBoolToObject(root, "enabled", 1);
    double newnum = cJSON_SetNumberHelper(num, 42.0);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("two_replaced");
    cJSON_bool replaced_ok = cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON *idx1 = cJSON_GetArrayItem(items, 1);
    char *idx1_str = cJSON_GetStringValue(idx1);
    cJSON *copied = cJSON_CreateString(idx1_str);
    cJSON_AddItemToObject(root, "copied_second", copied);
    (void)newnum;
    (void)replaced_ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}