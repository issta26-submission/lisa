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
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "  {\n    \"name\" : \"example\",\n    \"meta\": { \"count\": 1 },\n    \"active\": false\n  }  ";
    size_t initial_len = sizeof(initial_json) - 1;
    char *mutable_json = (char *)cJSON_malloc(initial_len + 1);
    memcpy(mutable_json, initial_json, initial_len + 1);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON *true_item = cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_free(mutable_json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}