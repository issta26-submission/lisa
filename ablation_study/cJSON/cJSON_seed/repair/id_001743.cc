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
//<ID> 1743
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child1 = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child1", child1);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(child1, "name", name);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(child1, "value", num);
    cJSON *child2 = cJSON_CreateString("leaf");
    cJSON_AddItemToObject(root, "child2", child2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "ok");

    // step 3: Operate and Validate
    cJSON *detached1 = cJSON_DetachItemFromObject(root, "child1");
    cJSON *detached2 = cJSON_DetachItemViaPointer(root, child2);
    cJSON_bool invalid_flag = cJSON_IsInvalid(detached1);
    const char *version = cJSON_Version();
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = version ? version[0] : 'Y';
    buffer[2] = invalid_flag ? '1' : '0';
    (void)buffer;
    (void)printed;
    (void)version;
    (void)invalid_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(detached1);
    cJSON_Delete(detached2);
    return 66; // API sequence test completed successfully
}