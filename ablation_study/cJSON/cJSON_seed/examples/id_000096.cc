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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *b_true = cJSON_CreateBool((cJSON_bool)1);
    cJSON *b_false = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToObject(config, "enabled", b_true);
    cJSON_AddItemToObject(config, "visible", b_false);

    // step 2: Configure references
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON_AddItemReferenceToArray(refs, b_true);
    cJSON_AddItemReferenceToArray(refs, b_false);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(refs);
    cJSON *config_lookup = cJSON_GetObjectItem(root, "config");
    cJSON *enabled_lookup = cJSON_GetObjectItem(config_lookup, "enabled");
    cJSON *summary = cJSON_CreateBool((cJSON_bool)count);
    cJSON_AddItemToObject(root, "has_refs", summary);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}