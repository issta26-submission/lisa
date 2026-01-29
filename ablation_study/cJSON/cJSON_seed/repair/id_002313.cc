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
//<ID> 2313
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.14159));
    cJSON_AddItemToArray(items, cJSON_CreateFalse());
    cJSON_AddStringToObject(meta, "type", "collection");
    cJSON_AddStringToObject(info, "owner", "user-99");

    // step 3: Operate
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *info_ref = cJSON_GetObjectItem(root, "info");
    cJSON *items_ref = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(items_ref);
    cJSON *second_item = cJSON_GetArrayItem(items_ref, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json[0];
    char *owner = cJSON_GetStringValue(cJSON_GetObjectItem(info_ref, "owner"));
    buffer[1] = owner[0];
    buffer[2] = (char)(count + (int)second_val);
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}