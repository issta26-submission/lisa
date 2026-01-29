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
//<ID> 1626
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddNumberToObject(root, "root_value", 3.1415);

    // step 2: Configure
    cJSON *s = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, s);

    // step 3: Operate and Validate
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    const char *strval = cJSON_GetStringValue(item1);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    double num0 = cJSON_GetNumberValue(item0);
    char buffer[32];
    memset(buffer, 0, sizeof(buffer));
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    buffer[0] = printed[0];
    buffer[1] = strval[0];
    buffer[2] = (char)('0' + ((int)num0 % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}