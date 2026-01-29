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
//<ID> 486
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

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2ref = cJSON_CreateStringReference("beta_ref");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToArray(list, s2ref);
    cJSON_AddItemToArray(list, s3);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(list, 0);
    const char *first_val = cJSON_GetStringValue(first_item);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s-updated", first_val ? first_val : "null");
    cJSON *replacement = cJSON_CreateString(buf);
    cJSON_ReplaceItemInArray(list, 0, replacement);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_ref = cJSON_CreateStringReference(snapshot);
    cJSON_AddItemToObject(root, "snapshot_ref", snapshot_ref);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(snapshot);
    // API sequence test completed successfully
    return 66;
}