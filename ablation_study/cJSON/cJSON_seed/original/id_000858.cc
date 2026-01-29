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
//<ID> 858
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    const char label_text[] = "example_label";
    cJSON *label_ref = cJSON_CreateStringReference(label_text);
    cJSON_AddItemToObject(meta, "label", label_ref);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.72);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    char prealloc_buf[256];
    memset(prealloc_buf, 0, sizeof(prealloc_buf));

    // step 3: Operate
    cJSON_bool ok = cJSON_PrintPreallocated(root, prealloc_buf, (int)sizeof(prealloc_buf), 1);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 1));
    double combined = v0 + v1;
    cJSON_SetNumberHelper(n1, combined);

    // step 4: Validate & Cleanup
    cJSON *replacement = cJSON_CreateNumber(1.0);
    cJSON_ReplaceItemInObject(root, "active", replacement);
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "label");
    (void)ok;
    (void)prealloc_buf;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}