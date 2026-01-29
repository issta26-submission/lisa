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
//<ID> 634
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
    cJSON *width = cJSON_CreateNumber(800.0);
    cJSON *height = cJSON_CreateNumber(600.0);
    cJSON_AddItemToObject(config, "width", width);
    cJSON_AddItemToObject(config, "height", height);
    cJSON *active = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", active);

    // step 2: Configure
    cJSON_SetNumberHelper(width, 1024.0);
    cJSON *scale = cJSON_CreateNumber(1.5);
    cJSON_AddItemToObject(config, "scale", scale);
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate and Validate
    cJSON *new_height = cJSON_CreateNumber(768.0);
    cJSON_ReplaceItemViaPointer(config, height, new_height);
    cJSON *w_item = cJSON_GetObjectItem(config, "width");
    cJSON *h_item = cJSON_GetObjectItem(config, "height");
    double w_val = cJSON_GetNumberValue(w_item);
    double h_val = cJSON_GetNumberValue(h_item);
    double area_val = w_val * h_val;
    cJSON *area_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(area_item, area_val);
    cJSON_AddItemToObject(root, "area", area_item);
    char buffer[512];
    memset(buffer, 0, sizeof buffer);
    cJSON_PrintPreallocated(root, buffer, (int)sizeof buffer, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}