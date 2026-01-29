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
//<ID> 1281
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
    cJSON *label = cJSON_CreateString("dataset");

    // step 2: Configure
    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "label", label);
    cJSON *items_ref = cJSON_Duplicate(items, cJSON_True); /* duplicate instead of creating a direct reference to avoid double-free */
    cJSON_AddItemToObject(root, "items_ref", items_ref);

    // step 3: Operate & Validate
    char *out = cJSON_Print(root);
    size_t out_len = strlen(out);
    char *dup = (char *)cJSON_malloc(out_len + 1);
    memset(dup, 0, out_len + 1);
    memcpy(dup, out, out_len + 1);
    cJSON_free(out);
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    cJSON_AddStringToObject(root, "label_copy", label_str);
    char *ref_out = cJSON_Print(items_ref);
    cJSON_free(ref_out);

    // step 4: Cleanup
    cJSON_free(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}