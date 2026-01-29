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
//<ID> 489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *orig = "ref-string";
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *sref = cJSON_CreateStringReference(orig);
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, sref);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    char summary_buf[128];
    memset(summary_buf, 0, sizeof(summary_buf));
    char *v0 = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    char *vref = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    sprintf(summary_buf, "first=%s;ref=%s", v0 ? v0 : "null", vref ? vref : "null");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "summary", summary_buf);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    (void)equal;
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_copy", dup_arr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}