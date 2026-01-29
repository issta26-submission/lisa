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
//<ID> 1309
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
    cJSON *item0 = cJSON_CreateString("first");
    cJSON *item1 = cJSON_CreateNumber(2.0);
    cJSON *replacement = cJSON_CreateString("replaced");

    // step 2: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *flag = cJSON_AddFalseToObject(root, "valid_flag");

    // step 3: Operate & Validate
    cJSON_bool did_replace = cJSON_ReplaceItemInArray(arr, 0, replacement);
    cJSON_bool was_invalid = cJSON_IsInvalid(item1);
    char *printed = cJSON_PrintUnformatted(root);
    size_t len = strlen(printed);
    char *copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, printed, len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}