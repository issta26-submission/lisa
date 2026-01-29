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
//<ID> 1892
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
    cJSON *status = cJSON_CreateString("init");
    cJSON_AddItemToObject(root, "status", status);

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.5);
    cJSON *num2 = cJSON_CreateNumber(-3.1415);
    cJSON_InsertItemInArray(items, 0, num0);
    cJSON_InsertItemInArray(items, 1, num1);
    cJSON_InsertItemInArray(items, 2, num2);

    // step 3: Operate and Validate
    cJSON *new_status = cJSON_CreateString("ready");
    cJSON_ReplaceItemInObject(root, "status", new_status);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    int digit = (int)second_value;
    int d = ((digit % 10) + 10) % 10;
    buffer[1] = (char)('0' + d);
    buffer[2] = (char)('0' + (cJSON_IsNumber(second_item) % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}