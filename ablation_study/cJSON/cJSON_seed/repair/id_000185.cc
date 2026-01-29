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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *pi = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(config, "pi", pi);
    cJSON *meta_raw = cJSON_AddRawToObject(config, "meta", "{\"enabled\":true}");

    // step 2: Configure
    cJSON *two = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "two", two);
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_pi = cJSON_GetObjectItem(got_config, "pi");

    // step 3: Operate and Validate
    cJSON *detached_pi = cJSON_DetachItemViaPointer(got_config, got_pi);
    cJSON_AddItemToObject(root, "moved_pi", detached_pi);
    double v_pi = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "moved_pi"));
    double v_two = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "two"));
    cJSON *sum_item = cJSON_CreateNumber(v_pi + v_two);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}