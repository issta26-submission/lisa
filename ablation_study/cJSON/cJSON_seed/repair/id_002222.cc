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
//<ID> 2222
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
    cJSON *temp = cJSON_CreateNumber(123.45);
    cJSON_AddItemToObject(root, "temp", temp);
    cJSON *name = cJSON_CreateString("device");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "list", list);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(list, n0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(list, n1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *author = cJSON_CreateString("alice");
    cJSON_AddItemToObject(meta, "author", author);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON_bool is_obj = cJSON_IsObject(got_config);
    (void)is_obj;
    cJSON *got_temp = cJSON_GetObjectItem(root, "temp");
    double temp_val = cJSON_GetNumberValue(got_temp);
    (void)temp_val;
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
    cJSON_DeleteItemFromObject(root, "temp");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}