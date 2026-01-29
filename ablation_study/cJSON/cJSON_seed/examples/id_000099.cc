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
//<ID> 99
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *original = cJSON_CreateString("original_value");
    cJSON_AddItemToObject(root, "original", original);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_AddItemReferenceToArray(refs, original);

    // step 3: Operate & Validate
    cJSON *got_refs = cJSON_GetObjectItem(root, "refs");
    int refs_count = cJSON_GetArraySize(got_refs);
    cJSON *count = cJSON_CreateNumber((double)refs_count);
    cJSON_AddItemToObject(root, "refs_count", count);
    cJSON *got_original = cJSON_GetObjectItem(root, "original");
    const char *orig_text = cJSON_GetStringValue(got_original);
    cJSON *label = cJSON_CreateString(orig_text ? orig_text : "");
    cJSON_AddItemToObject(root, "label", label);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}