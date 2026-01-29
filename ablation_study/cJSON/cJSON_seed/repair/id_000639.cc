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
//<ID> 639
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
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(settings, "enabled", enabled);
    const char *ver_str = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver_str);
    cJSON_AddItemToObject(root, "version", ver_item);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *v0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, v0);

    // step 2: Configure
    double set_res = cJSON_SetNumberHelper(threshold, 20.5);
    cJSON *replacement_threshold = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(settings, threshold, replacement_threshold);
    double set_res2 = cJSON_SetNumberHelper(replacement_threshold, 84.0);

    // step 3: Operate and Validate
    cJSON *read_thresh = cJSON_GetObjectItem(settings, "threshold");
    double thresh_value = cJSON_GetNumberValue(read_thresh);
    cJSON_AddNumberToObject(root, "threshold_readback", thresh_value);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;
    cJSON_AddStringToObject(root, "printed_snapshot", buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}