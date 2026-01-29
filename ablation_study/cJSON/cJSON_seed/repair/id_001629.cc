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
//<ID> 1629
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
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON *s2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    int initial_count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)initial_count);
    cJSON *replacement = cJSON_CreateString("replaced");

    // step 3: Operate and Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    char *printed = cJSON_PrintBuffered(root, 32, 1);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = second_str ? second_str[0] : 'X';
    buffer[1] = (char)('0' + (int)replaced);
    buffer[2] = printed ? printed[0] : 'Y';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}