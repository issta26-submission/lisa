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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *initial_tags[] = { "alpha", "beta", "gamma" };
    cJSON *tags = cJSON_CreateStringArray(initial_tags, 3);
    cJSON_AddItemToObject(root, "tags", tags);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *owner = cJSON_CreateString("owner_name");
    cJSON_AddItemToObject(meta, "owner", owner);

    // step 2: Configure
    const char *replacement_tags[] = { "delta", "epsilon" };
    cJSON *new_tags = cJSON_CreateStringArray(replacement_tags, 2);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "tags", new_tags);
    cJSON *replaced_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replaced_flag", replaced_flag);

    // step 3: Operate & Validate
    cJSON *got_tags = cJSON_GetObjectItem(root, "tags");
    cJSON *first_tag = cJSON_GetArrayItem(got_tags, 0);
    char *first_tag_str = cJSON_GetStringValue(first_tag);
    cJSON *first_tag_copy = cJSON_CreateString(first_tag_str);
    cJSON_AddItemToObject(root, "first_tag_copy", first_tag_copy);
    const int buf_len = 256;
    char buffer[buf_len];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}