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
//<ID> 186
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
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *raw_added = cJSON_AddRawToObject(config, "raw_data", "{\"inner\":123}");
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "num1", num1);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(metrics, "pi", pi);

    // step 2: Configure
    cJSON *found_config = cJSON_GetObjectItem(root, "config");
    cJSON *raw_item = cJSON_GetObjectItem(found_config, "raw_data");
    (void)raw_item; // use to indicate it's intentionally obtained for flow

    // step 3: Operate and Validate
    cJSON *target = cJSON_GetObjectItem(config, "num1");
    cJSON *detached = cJSON_DetachItemViaPointer(config, target);
    cJSON_AddItemToObject(metrics, "orphan", detached);
    double v_orphan = cJSON_GetNumberValue(detached);
    double v_pi = cJSON_GetNumberValue(pi);
    double total = v_orphan + v_pi;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}