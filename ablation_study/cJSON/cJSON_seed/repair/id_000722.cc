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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    char *s2_new = cJSON_SetValuestring(s2, "beta");
    cJSON_AddStringToObject(meta, "s2_new", s2_new);
    cJSON *s3 = cJSON_CreateString("delta");
    cJSON_AddItemToArray(items, s3);

    // step 3: Operate and Validate
    cJSON *items_ref = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *meta_ref = cJSON_GetObjectItemCaseSensitive(root, "meta");
    int total = cJSON_GetArraySize(items_ref);
    cJSON_AddNumberToObject(root, "count", (double)total);
    cJSON *middle = cJSON_GetArrayItem(items_ref, 1);
    char *middle_val = cJSON_GetStringValue(middle);
    cJSON_AddStringToObject(meta_ref, "middle", middle_val);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}