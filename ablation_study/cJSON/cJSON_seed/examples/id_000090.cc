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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *refs = cJSON_CreateArray();
    cJSON *b_enabled = cJSON_CreateBool(1);
    cJSON *b_visible = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "enabled", b_enabled);
    cJSON_AddItemToObject(root, "visible", b_visible);
    cJSON_AddItemToObject(root, "bool_refs", refs);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(refs, b_enabled);
    cJSON_AddItemReferenceToArray(refs, b_visible);

    // step 3: Operate & Validate
    cJSON *retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *retrieved_refs = cJSON_GetObjectItem(root, "bool_refs");
    int ref_count = cJSON_GetArraySize(retrieved_refs);
    cJSON *enabled_copy = cJSON_Duplicate(retrieved_enabled, 1);
    cJSON_AddItemToObject(root, "enabled_copy", enabled_copy);
    cJSON *count = cJSON_CreateNumber((double)ref_count);
    cJSON_AddItemToObject(root, "ref_count", count);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}