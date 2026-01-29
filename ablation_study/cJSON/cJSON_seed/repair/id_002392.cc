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
//<ID> 2392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *temp = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_temp = cJSON_HasObjectItem(root, "temp");
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "temp");
    char *updated = cJSON_SetValuestring(detached, "newval");
    cJSON_AddItemToArray(arr, detached);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = updated ? updated[0] : '\0';
    buffer[2] = (char)(int)(has_temp + has_data);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}