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
//<ID> 188
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
    cJSON *one = cJSON_CreateString("one");
    cJSON *two = cJSON_CreateString("two");
    cJSON *three = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, one);
    cJSON_AddItemToArray(arr, two);
    cJSON_AddItemToArray(arr, three);

    // step 2: Configure
    cJSON *repl = cJSON_CreateStringReference("replaced");
    cJSON_ReplaceItemInArray(arr, 1, repl);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "key", cJSON_CreateNumber(99.0));
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "child");
    cJSON_AddItemToArray(arr, detached);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}