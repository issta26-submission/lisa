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
//<ID> 1628
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "total", 42.0);
    cJSON *replacement = cJSON_CreateString("beta_replaced");

    // step 3: Operate and Validate
    cJSON_bool did_replace = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *middle = cJSON_GetArrayItem(arr, 1);
    char *mid_str = cJSON_GetStringValue(middle);
    cJSON *total_item = cJSON_GetObjectItem(root, "total");
    double total_val = cJSON_GetNumberValue(total_item);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    char buffer[8];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = mid_str[0];
    buffer[1] = (char)('0' + ((int)total_val % 10));
    buffer[2] = printed[0];
    buffer[3] = (char)('0' + (int)did_replace);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}