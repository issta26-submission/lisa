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
//<ID> 1653
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int base_numbers[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateIntArray(base_numbers, 4);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *extra = cJSON_CreateArray();
    cJSON *e0 = cJSON_CreateNumber(7.0);
    cJSON *e1 = cJSON_CreateNumber(8.0);
    cJSON_AddItemToArray(extra, e0);
    cJSON_AddItemToArray(extra, e1);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON_bool hasNumbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *num_item_1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *extra_item_0 = cJSON_GetArrayItem(extra, 0);
    double v_num1 = cJSON_GetNumberValue(num_item_1);
    double v_extra0 = cJSON_GetNumberValue(extra_item_0);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = (char)('0' + ((int)v_num1 % 10));
    buffer[1] = (char)('0' + ((int)v_extra0 % 10));
    buffer[2] = (char)('0' + (int)hasNumbers);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[3] = printed[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}