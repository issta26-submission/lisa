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
//<ID> 1895
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
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *title = cJSON_CreateString("old_title");
    cJSON_AddItemToObject(root, "title", title);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *num2 = cJSON_CreateNumber(21.0);
    cJSON_InsertItemInArray(arr, 1, num2);
    cJSON *num3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num3);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    double got_val = cJSON_GetNumberValue(got);
    cJSON *new_title = cJSON_CreateString("new_title");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "title", new_title);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + ((int)got_val % 10));
    buffer[2] = (char)('0' + (replaced % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}