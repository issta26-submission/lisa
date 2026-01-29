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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"user\":{\"name\":\"alice\",\"tags\":[\"alpha\",\"beta\"]}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON *user = cJSON_GetObjectItemCaseSensitive(root, "user");
    cJSON_bool user_is_obj = cJSON_IsObject(user);
    cJSON *tags = cJSON_GetObjectItemCaseSensitive(user, "tags");
    cJSON_bool tags_invalid = cJSON_IsInvalid(tags);
    cJSON *new_tags = cJSON_CreateArray();
    cJSON *ref_gamma = cJSON_CreateStringReference("gamma");
    cJSON *ref_delta = cJSON_CreateStringReference("delta");
    cJSON_AddItemToArray(new_tags, ref_gamma);
    cJSON_AddItemToArray(new_tags, ref_delta);
    cJSON_AddItemToObject(root, "added_tags", new_tags);

    // step 3: Operate
    int size_before = cJSON_GetArraySize(tags);
    cJSON *ref_epsilon = cJSON_CreateStringReference("epsilon");
    cJSON_AddItemToArray(tags, ref_epsilon);
    cJSON *ref_zeta = cJSON_CreateStringReference("zeta");
    cJSON_AddItemReferenceToArray(tags, ref_zeta);
    int size_after = cJSON_GetArraySize(tags);
    int delta = size_after - size_before;
    cJSON_AddNumberToObject(root, "delta", (double)delta);
    const char *last_tag = cJSON_GetStringValue(cJSON_GetArrayItem(tags, size_after - 1));
    (void)last_tag;
    (void)user_is_obj;
    (void)tags_invalid;

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}