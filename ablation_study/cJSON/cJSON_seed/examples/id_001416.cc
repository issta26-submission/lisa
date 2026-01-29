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
//<ID> 1416
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
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *primary_item = cJSON_AddNumberToObject(root, "primary", 42.0);
    cJSON *active_item = cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 2: Configure
    cJSON_AddNumberToObject(child_obj, "score", 3.14);
    cJSON_AddItemReferenceToArray(refs, primary_item);
    cJSON_AddItemReferenceToArray(refs, child_obj);

    // step 3: Operate & Validate
    cJSON *dup_refs = cJSON_Duplicate(refs, 1);
    cJSON_AddItemToObject(root, "refs_copy", dup_refs);
    cJSON *got_primary = cJSON_GetObjectItem(root, "primary");
    double primary_val = cJSON_GetNumberValue(got_primary);
    cJSON_AddNumberToObject(root, "primary_plus", primary_val + 10.0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}