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
//<ID> 339
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
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta_orig = cJSON_CreateObject();
    cJSON_AddStringToObject(meta_orig, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta_orig);

    // step 2: Configure
    cJSON *meta_new = cJSON_CreateObject();
    cJSON_AddStringToObject(meta_new, "version", "2.0");
    cJSON_AddStringToObject(meta_new, "status", "updated");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", meta_new);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *first_double = cJSON_CreateNumber(first_val * 2.0);
    cJSON_AddItemToObject(root, "first_double", first_double);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}