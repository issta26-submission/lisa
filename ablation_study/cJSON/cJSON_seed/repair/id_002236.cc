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
//<ID> 2236
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
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(cfg, "name", name);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(cfg, "count", count);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(cfg, "items", arr);
    cJSON *item0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, item0);
    cJSON *item1 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, item1);

    // step 3: Operate and Validate
    cJSON *got_cfg = cJSON_GetObjectItem(root, "config");
    cJSON *got_count = cJSON_GetObjectItem(got_cfg, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    (void)count_val;
    cJSON *got_name = cJSON_GetObjectItem(got_cfg, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    (void)name_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}