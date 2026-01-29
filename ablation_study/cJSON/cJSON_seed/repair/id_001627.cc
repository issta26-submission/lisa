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
//<ID> 1627
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
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(100.0);
    cJSON *str0 = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddNumberToObject(root, "root_value", 3.1415);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    char *got_str = cJSON_GetStringValue(got);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = got_str[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}