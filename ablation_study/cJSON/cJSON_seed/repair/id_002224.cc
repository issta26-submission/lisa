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
//<ID> 2224
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(settings, "count", count);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(settings, "name", name);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(count, 10.0);
    (void)new_val;
    char *new_name = cJSON_SetValuestring(name, "updated");
    (void)new_name;
    cJSON_bool is_obj = cJSON_IsObject(settings);
    (void)is_obj;

    // step 3: Operate and Validate
    cJSON *got_count = cJSON_GetObjectItem(settings, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    (void)count_val;
    cJSON *got_name = cJSON_GetObjectItem(settings, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    (void)name_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "settings");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}