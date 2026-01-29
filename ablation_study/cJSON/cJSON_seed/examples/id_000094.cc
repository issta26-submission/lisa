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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *active = cJSON_CreateBool(1);
    cJSON *enabled = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(refs, active);
    cJSON_AddItemReferenceToArray(refs, enabled);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *tag = cJSON_CreateString("ref_test");
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate & Validate
    cJSON *got_refs = cJSON_GetObjectItem(root, "refs");
    int ref_count = cJSON_GetArraySize(got_refs);
    cJSON *count_item = cJSON_CreateNumber((double)ref_count);
    cJSON_AddItemToObject(root, "ref_count", count_item);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    (void)got_active;
    (void)got_enabled;

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}