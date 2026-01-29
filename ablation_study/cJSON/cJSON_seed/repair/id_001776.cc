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
//<ID> 1776
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
    cJSON_AddStringToObject(user, "name", "Alice");
    cJSON_AddNumberToObject(user, "age", 29.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "env", "testing");
    cJSON_AddStringToObject(root, "version", cJSON_Version());

    // step 3: Operate and Validate
    cJSON *user_item = cJSON_GetObjectItem(root, "user");
    cJSON *name_item = cJSON_GetObjectItem(user_item, "name");
    cJSON *age_item = cJSON_GetObjectItem(user_item, "age");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    char *name_str = cJSON_GetStringValue(name_item);
    double age_val = cJSON_GetNumberValue(age_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_is_string ? (name_str ? name_str[0] : 'Z') : 'N';
    buffer[2] = (char)('0' + ((int)age_val % 10));
    (void)buffer;
    (void)printed;
    (void)name_str;
    (void)age_val;
    (void)name_is_string;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}