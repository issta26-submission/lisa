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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "[\"alpha\",\"beta\"]";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);

    // step 2: Configure
    cJSON *item0 = cJSON_GetArrayItem(root, 0);
    cJSON *item1 = cJSON_GetArrayItem(root, 1);
    cJSON *dup_item1 = cJSON_Duplicate(item1, 1);
    char *updated = cJSON_SetValuestring(dup_item1, "gamma");
    (void)updated;
    cJSON_AddItemToArray(root, dup_item1);

    // step 3: Operate and Validate
    cJSON *got2 = cJSON_GetArrayItem(root, 2);
    const char *v0 = cJSON_GetStringValue(item0);
    const char *v2 = cJSON_GetStringValue(got2);
    double lsum = 0.0;
    lsum = (double)((v0 != NULL ? (int)strlen(v0) : 0) + (v2 != NULL ? (int)strlen(v2) : 0));
    cJSON *sum_num = cJSON_CreateNumber(lsum);
    cJSON_AddItemToArray(root, sum_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}