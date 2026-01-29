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
//<ID> 1291
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
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON *str_b = cJSON_CreateString("beta");
    cJSON *flag_true = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "valid", flag_true);
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemReferenceToObject(root, "items_ref", items_dup);
    cJSON *label = cJSON_CreateString("dataset");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON_bool is_true = cJSON_IsTrue(flag_true);
    (void)is_true;
    int count = cJSON_GetArraySize(items);
    (void)count;
    char *printed = cJSON_PrintUnformatted(root);
    const char *val = cJSON_GetStringValue(str_a);
    size_t len = 0;
    while (val && val[len]) { len++; } /* simple length computation without using strlen */
    char *copy = (char *)cJSON_malloc(len + 1);
    if (copy) { memcpy(copy, val, len + 1); }
    cJSON_free(printed);
    cJSON_free(copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(items_dup);

    // API sequence test completed successfully
    return 66;
}