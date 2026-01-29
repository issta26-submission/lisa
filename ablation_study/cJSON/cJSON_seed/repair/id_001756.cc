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
//<ID> 1756
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.5));
    cJSON_AddItemToObject(meta, "status", cJSON_CreateString("ready"));

    // step 2: Configure
    cJSON_AddItemToObject(root, "mode", cJSON_CreateString("production"));
    cJSON_AddItemToObject(root, "features", cJSON_CreateArray());
    cJSON *features = cJSON_GetObjectItem(root, "features");
    cJSON_AddItemToArray(features, cJSON_CreateString("logging"));
    cJSON_AddItemToArray(features, cJSON_CreateString("metrics"));

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool same = cJSON_Compare(root, duplicate, 1);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char *status_str = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "status"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)same);
    buffer[2] = status_str ? status_str[0] : 'Z';
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}