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
//<ID> 1012
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"device\":{\"name\":\"sensor\",\"readings\":[12.5,7.0,3.25]}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *device = cJSON_GetObjectItem(root, "device");
    cJSON *readings = cJSON_GetObjectItem(device, "readings");

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(readings, 1);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(readings, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *name_item = cJSON_GetObjectItem(device, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "first_value", cJSON_CreateNumber(first_val));
    cJSON_AddItemToObject(summary, "device_name", cJSON_CreateString(name_str));
    cJSON_AddItemToObject(summary, "detached_reading", detached);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}