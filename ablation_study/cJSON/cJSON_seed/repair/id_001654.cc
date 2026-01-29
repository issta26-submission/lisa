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
//<ID> 1654
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int base_numbers[] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *ints = cJSON_CreateIntArray(base_numbers, 4);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *extras = cJSON_CreateArray();
    cJSON *extra_a = cJSON_CreateNumber(99.0);
    cJSON *extra_b = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(extras, extra_a);
    cJSON_AddItemToArray(extras, extra_b);
    cJSON_AddItemToObject(root, "extras", extras);

    // step 2: Configure
    cJSON_bool has_ints = cJSON_HasObjectItem(root, "ints");
    cJSON *ints_obj = cJSON_GetObjectItem(root, "ints");
    cJSON *third_item = cJSON_GetArrayItem(ints_obj, 2);
    double third_value = cJSON_GetNumberValue(third_item);
    cJSON *computed = cJSON_CreateNumber(third_value + 5.5);
    cJSON_AddItemToArray(extras, computed);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)has_ints);
    buffer[2] = (char)('0' + ((int)third_value % 10));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}