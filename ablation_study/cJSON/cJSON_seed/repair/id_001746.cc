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
//<ID> 1746
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
    cJSON *name = cJSON_CreateString("fuzzer");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *value = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(config, "value", value);
    const char *ver = cJSON_Version();
    cJSON *ver_node = cJSON_CreateString(ver ? ver : "unknown");
    cJSON_AddItemToObject(root, "version", ver_node);

    // step 2: Configure
    cJSON *detached_by_name = cJSON_DetachItemFromObject(config, "value");
    cJSON *name_ptr = cJSON_GetObjectItem(config, "name");
    cJSON *detached_by_ptr = cJSON_DetachItemViaPointer(config, name_ptr);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool inv1 = cJSON_IsInvalid(detached_by_name);
    cJSON_bool inv2 = cJSON_IsInvalid(detached_by_ptr);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver ? ver[0] : 'Y';
    buffer[2] = (char)('0' + (int)inv1);
    buffer[3] = (char)('0' + (int)inv2);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_by_name);
    cJSON_Delete(detached_by_ptr);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}