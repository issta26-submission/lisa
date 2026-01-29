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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(list, first);
    cJSON *ref = cJSON_CreateStringReference("ref_string");
    cJSON_AddItemToArray(list, ref);
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(list, num);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(list, 1, replacement);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *meta_ref = cJSON_CreateStringReference("meta_ref");
    cJSON_AddItemToObject(meta, "ref", meta_ref);

    // step 3: Operate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}