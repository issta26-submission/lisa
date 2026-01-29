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
//<ID> 1622
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
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *replacement = cJSON_CreateNumber(second_val * 10.0);
    cJSON_ReplaceItemInArray(arr, 0, replacement);

    // step 3: Operate and Validate
    cJSON *first_after_replace = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_after_replace);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed && printed[0] ? printed[0] : 0;
    buffer[1] = (char)('0' + ((int)first_val % 10));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}