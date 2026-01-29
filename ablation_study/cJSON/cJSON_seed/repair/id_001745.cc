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
//<ID> 1745
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(payload, "name", name);
    cJSON *value = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(payload, "value", value);

    // step 2: Configure
    const char *version_str = cJSON_Version();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ver_node = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(meta, "version", ver_node);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version_str ? version_str[0] : 'Y';
    cJSON *name_ptr = cJSON_GetObjectItem(payload, "name");
    cJSON *detached_name = cJSON_DetachItemViaPointer(payload, name_ptr);
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_bool name_is_invalid = cJSON_IsInvalid(detached_name);
    cJSON_bool meta_is_invalid = cJSON_IsInvalid(detached_meta);
    buffer[2] = (char)('0' + (int)name_is_invalid);
    buffer[3] = (char)('0' + (int)meta_is_invalid);
    (void)buffer;
    (void)name_is_invalid;
    (void)meta_is_invalid;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_name);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}