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
//<ID> 1302
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
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("second_replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate & Validate
    char *printed_root = cJSON_PrintUnformatted(root);
    cJSON_bool is_invalid_root = cJSON_IsInvalid(root);
    char *printed_arr = cJSON_PrintUnformatted(arr);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_arr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}