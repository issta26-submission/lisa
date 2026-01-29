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
//<ID> 744
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    (void)version;
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *num_item = cJSON_CreateNumber(123.0);

    // step 2: Configure
    cJSON_AddItemToObject(child, "nullable", null_item);
    cJSON_AddItemToObject(child, "value", num_item);
    cJSON_AddItemToObject(root, "data", child);
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("example"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *data_obj = cJSON_GetObjectItem(parsed, "data");
    (void)data_obj;
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed, "data");
    cJSON *value_item = cJSON_GetObjectItem(detached, "value");
    double value = cJSON_GetNumberValue(value_item);
    (void)value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}