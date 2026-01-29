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
//<ID> 488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"Alice\",\"age\":30,\"active\":false}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json), &parse_end, 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *age_item = cJSON_GetObjectItem(root, "age");
    double age_val = cJSON_GetNumberValue(age_item);

    // step 2: Configure
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    cJSON *name_flag = cJSON_CreateBool(name_is_str);
    cJSON_AddItemToObject(root, "name_is_string", name_flag);
    cJSON *new_active = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObject(root, "active", new_active);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snapshot_len = strlen(snapshot);
    char *copy = (char *)cJSON_malloc(snapshot_len + 1);
    memcpy(copy, snapshot, snapshot_len + 1);
    double active_true = (double)cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    double validation_score = age_val + (double)name_is_str + active_true;

    // step 4: Validate & Cleanup
    (void)validation_score;
    cJSON_free(snapshot);
    cJSON_free(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}