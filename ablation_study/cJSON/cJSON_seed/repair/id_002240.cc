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
//<ID> 2240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON_AddItemToObject(cfg, "name", name);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "readings", arr);
    cJSON *r0 = cJSON_CreateNumber(12.5);
    cJSON_AddItemToArray(arr, r0);
    cJSON *r1 = cJSON_CreateNumber(7.25);
    cJSON_AddItemToArray(arr, r1);
    cJSON *r2 = cJSON_CreateNumber(3.125);
    cJSON_AddItemToArray(arr, r2);
    cJSON *topnum = cJSON_AddNumberToObject(root, "threshold", 5.0);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *thr = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = name_val ? name_val[0] : '\0';
    buffer[2] = (char)((int)thr_val % 256);
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}