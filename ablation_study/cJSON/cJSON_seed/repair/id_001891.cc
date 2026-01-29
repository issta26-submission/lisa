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
//<ID> 1891
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
    cJSON *n0 = cJSON_CreateNumber(7.0);
    cJSON_InsertItemInArray(items, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(items, 1, n1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root, "name", new_name);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + ((int)second_val % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}