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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 2: Configure
    cJSON *alias_null = cJSON_CreateObject();
    cJSON_AddItemToObject(alias_null, "alias", cJSON_CreateNull());
    cJSON_AddItemToObject(root, "alias_container", alias_null);

    // step 3: Operate and Validate
    char *json_unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_unformatted);
    size_t len = strlen(json_unformatted);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, json_unformatted, len + 1);
    cJSON *retrieved = cJSON_GetObjectItem(root, "empty");
    cJSON_bool is_null = cJSON_IsNull(retrieved);
    int summary = (int)len + (int)is_null + root->type + retrieved->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(json_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}