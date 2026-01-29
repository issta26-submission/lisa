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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_AddNullToObject(root, "null_key");
    cJSON *true_item = cJSON_AddTrueToObject(root, "true_key");
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_key", "{\"raw\":1}");

    // step 2: Operate
    cJSON *got_null = cJSON_GetObjectItem(root, "null_key");
    cJSON *got_true = cJSON_GetObjectItem(root, "true_key");
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw_key");
    cJSON_bool is_true = cJSON_IsTrue(got_true);
    cJSON_bool is_raw = cJSON_IsRaw(got_raw);

    // step 3: Validate
    int validation_flags = (int)is_true + ((int)is_raw << 1);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_AddNumberToObject(root, "validation", (double)validation_flags);
    char *printed2 = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}