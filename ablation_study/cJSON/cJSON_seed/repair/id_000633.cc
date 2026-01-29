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
//<ID> 633
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *count = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(metrics, "count", count);
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "active", flag);

    // step 2: Configure
    double prev_count = cJSON_SetNumberHelper(count, 20.0);
    cJSON *ratio = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(metrics, "ratio", ratio);
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate and Validate
    cJSON *new_flag = cJSON_CreateBool(1);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, flag, new_flag);
    cJSON *count_ref = cJSON_GetObjectItem(metrics, "count");
    double current_count = cJSON_GetNumberValue(count_ref);
    double adjusted = current_count * cJSON_GetNumberValue(ratio) + prev_count;
    cJSON_AddNumberToObject(metrics, "adjusted", adjusted);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}