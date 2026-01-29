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
//<ID> 1288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON *str_b = cJSON_CreateString("beta");

    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);

    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON *items_ref = cJSON_CreateArrayReference(dup_items);
    cJSON_AddItemToObject(root, "items_ref", items_ref);

    char *dyn = (char*)cJSON_malloc(64);
    memset(dyn, 0, 64);
    const char *label = "dynamic_label";
    memcpy(dyn, label, strlen(label) + 1);
    cJSON *dyn_str = cJSON_CreateString(dyn);
    cJSON_AddItemToObject(root, "label", dyn_str);

    char *got = (char*)cJSON_GetStringValue(str_b);
    size_t got_len = strlen(got);
    char *got_copy = (char*)cJSON_malloc(got_len + 1);
    memset(got_copy, 0, got_len + 1);
    memcpy(got_copy, got, got_len + 1);
    char *out = cJSON_Print(root);

    cJSON_free(out);
    cJSON_free(got_copy);
    cJSON_free(dyn);
    cJSON_Delete(root);

    return 66;
}