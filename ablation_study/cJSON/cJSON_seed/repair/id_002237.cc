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
//<ID> 2237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *mode = cJSON_CreateString("fast");
    cJSON_AddItemToObject(config, "mode", mode);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(config, "count", count);
    cJSON *enabled = cJSON_CreateBool((cJSON_bool)1);
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Operate & Validate
    cJSON *got_mode = cJSON_GetObjectItem(config, "mode");
    char *mode_str = cJSON_GetStringValue(got_mode);
    cJSON *got_count = cJSON_GetObjectItem(config, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON *next_count = cJSON_CreateNumber(count_val + 1.0);
    cJSON_AddItemToObject(root, "nextCount", next_count);
    cJSON *status = cJSON_CreateString(mode_str);
    cJSON_AddItemToObject(root, "statusMode", status);
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

    // step 3: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}