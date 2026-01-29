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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "name", "test_doc");
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "author", "alice");
    cJSON_AddNumberToObject(meta, "revision", 3.0);
    cJSON *optional_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional_null);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate and Validate
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    cJSON_AddNumberToObject(root, "version_times_two", ver * 2.0);
    cJSON_bool active_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    cJSON_AddNumberToObject(root, "active_flag", (double)active_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}