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
//<ID> 1307
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
    cJSON *item0 = cJSON_CreateString("one");
    cJSON *item1 = cJSON_CreateString("two");
    cJSON *new_item = cJSON_CreateString("replaced");

    // step 2: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate & Validate
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 0, new_item);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_bool invalid_check = cJSON_IsInvalid(item1);
    size_t out_len = strlen(out);
    char *copy = (char *)cJSON_malloc(out_len + 1);
    memcpy(copy, out, out_len + 1);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}