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
//<ID> 886
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
    cJSON *temp = cJSON_CreateNumber(22.0);
    cJSON *active = cJSON_CreateBool(1);
    cJSON *threshold = cJSON_CreateNumber(30.5);
    cJSON *count = cJSON_CreateNumber(3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "temperature", temp);
    cJSON_AddItemToObject(root, "active", active);
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObject(meta, "threshold", threshold);
    cJSON_AddItemToObjectCS(meta, "count", count);

    // step 3: Operate & Validate
    cJSON *got_temp = cJSON_GetObjectItem(root, "temperature");
    double orig_temp = cJSON_GetNumberValue(got_temp);
    double updated_value = cJSON_SetNumberHelper(got_temp, orig_temp + 1.5);
    cJSON *incremented = cJSON_CreateNumber(updated_value);
    cJSON_AddItemToObjectCS(root, "temperature_incremented", incremented);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *active_flag = cJSON_CreateBool(cJSON_IsTrue(active_item));
    cJSON_AddItemToObject(root, "active_flag", active_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}