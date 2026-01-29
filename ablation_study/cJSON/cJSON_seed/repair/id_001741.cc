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
//<ID> 1741
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "user", user);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(user, "name", name);
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(user, "age", age);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(user, "active", active);
    cJSON *temp = cJSON_CreateString("to_be_detached");
    cJSON_AddItemToObject(user, "temp", temp);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "configured");
    cJSON *note = cJSON_CreateString("initialized");
    cJSON_AddItemToObject(meta, "note", note);

    // step 3: Operate and Validate
    cJSON *retr_user = cJSON_GetObjectItem(root, "user");
    cJSON *retr_temp = cJSON_GetObjectItem(retr_user, "temp");
    cJSON *detached_temp = cJSON_DetachItemViaPointer(retr_user, retr_temp);
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_bool temp_invalid = cJSON_IsInvalid(detached_temp);
    char *printed = cJSON_PrintUnformatted(root);
    const char *version = cJSON_Version();
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version ? version[0] : 'V';
    buffer[2] = (char)('0' + (int)temp_invalid);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_temp);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}