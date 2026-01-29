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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(data, "optional", null_item);
    cJSON *raw_blob = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(data, "raw_blob", raw_blob);
    cJSON_AddStringToObject(meta, "author", "fuzzer");

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_raw = cJSON_GetObjectItem(got_data, "raw_blob");
    cJSON *got_optional = cJSON_GetObjectItem(got_data, "optional");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}