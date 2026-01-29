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
//<ID> 1740
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
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", arr);
    cJSON *v1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, v1);
    cJSON *v2 = cJSON_CreateString("keep");
    cJSON_AddItemToArray(arr, v2);
    cJSON *to_detach = cJSON_CreateString("detach_me");
    cJSON_AddItemToObject(config, "to_detach", to_detach);
    cJSON *byname = cJSON_CreateString("remove_by_name");
    cJSON_AddItemToObject(root, "remove", byname);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(meta, "count", num);

    // step 3: Operate and Validate
    cJSON *ptr_to_detach = cJSON_GetObjectItem(config, "to_detach");
    cJSON *detached_ptr = cJSON_DetachItemViaPointer(config, ptr_to_detach);
    cJSON *detached_byname = cJSON_DetachItemFromObject(root, "remove");
    cJSON_bool invalid1 = cJSON_IsInvalid(detached_ptr);
    cJSON_bool invalid2 = cJSON_IsInvalid(detached_byname);
    const char *ver = cJSON_Version();
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver ? ver[0] : 'Y';
    buffer[2] = (char)('0' + (int)(invalid1 + invalid2));
    buffer[3] = (char)('0' + (int)cJSON_GetArraySize(arr));
    (void)buffer;
    (void)ver;
    (void)invalid1;
    (void)invalid2;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_ptr);
    cJSON_Delete(detached_byname);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}