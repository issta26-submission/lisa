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
//<ID> 2399
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
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToArray(items, num_item);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "initial_name");

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];

    // step 3: Operate
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *detached = cJSON_DetachItemFromObject(root, "name");
    char *newval = cJSON_SetValuestring(detached, "renamed");
    cJSON_AddItemToArray(items, detached);
    buffer[1] = (char)(int)has_items;
    buffer[2] = (char)(int)has_name;
    buffer[3] = newval[0];

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[4] = printed2[0];
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}