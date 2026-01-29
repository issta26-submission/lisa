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
//<ID> 1620
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
    cJSON *s0 = cJSON_CreateString("zero");
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *count_ptr = cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[16];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = cJSON_GetStringValue(second)[0];
    buffer[2] = (char)('0' + (int)replaced);
    buffer[3] = (char)('0' + (int)count_ptr->valueint);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}