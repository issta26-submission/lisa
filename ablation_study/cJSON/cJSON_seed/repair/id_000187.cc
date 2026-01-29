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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *num_a = cJSON_CreateNumber(12.5);
    cJSON_AddItemToObject(settings, "a", num_a);
    cJSON *raw = cJSON_AddRawToObject(settings, "raw_blob", "{\"flag\":true}");

    // step 2: Configure
    cJSON *num_b = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "b", num_b);
    cJSON *found_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *found_a = cJSON_GetObjectItem(found_settings, "a");
    double val_a = cJSON_GetNumberValue(found_a);
    double val_b = cJSON_GetNumberValue(num_b);
    double sum = val_a + val_b;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate
    cJSON *detached_raw = cJSON_DetachItemViaPointer(settings, raw);
    cJSON_AddItemToObject(root, "detached_raw", detached_raw);
    cJSON *verify_detached = cJSON_GetObjectItem(root, "detached_raw");
    (void)verify_detached; // keep usage explicit for flow

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}