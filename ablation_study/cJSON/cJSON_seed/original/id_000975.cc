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
//<ID> 975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_with_ws[] = "{\n  \"flag\": false,\n  \"count\": 42,\n  \"nested\": { \"a\": 1 }\n}\n";
    size_t json_len = sizeof(json_with_ws) - 1;
    char *mutable_json = (char *)cJSON_malloc(json_len + 1);
    memcpy(mutable_json, json_with_ws, json_len + 1);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *existing_flag = cJSON_GetObjectItem(root, "flag");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "explicit_null", null_item);
    cJSON *true_item = cJSON_AddTrueToObject(root, "added_true");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON_PrintPreallocated(root, buffer, 512, 1);

    // step 4: Validate & Cleanup
    cJSON *reparsed = cJSON_Parse(out);
    cJSON_bool are_equal = cJSON_Compare(root, reparsed, 1);
    (void)are_equal;
    cJSON_Delete(reparsed);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_free(mutable_json);

    // API sequence test completed successfully
    return 66;
}