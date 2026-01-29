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
//<ID> 2390
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
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON *str = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", str);

    // step 2: Configure
    char *printed1 = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_name_before = cJSON_HasObjectItem(root, "name");
    buffer[0] = printed1 && printed1[0] ? printed1[0] : '\0';
    buffer[1] = (char)(int)has_name_before;

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "name");
    char *newval = cJSON_SetValuestring(detached, "moved");
    cJSON_AddItemToArray(arr, detached);
    cJSON_bool has_name_after = cJSON_HasObjectItem(root, "name");
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[2] = printed2 && printed2[0] ? printed2[0] : '\0';
    buffer[3] = (char)(int)has_name_after;

    // step 4: Validate and Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}