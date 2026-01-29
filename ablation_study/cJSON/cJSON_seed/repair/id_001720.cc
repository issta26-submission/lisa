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
//<ID> 1720
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "value", num_item);
    double set_val = cJSON_SetNumberHelper(num_item, 123.456);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, s1);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)cJSON_GetNumberValue(num_item) % 10));
    buffer[2] = (char)('F' + cJSON_IsTrue(flag) * ('T' - 'F'));
    buffer[3] = (char)('0' + (cJSON_GetArraySize(arr) % 10));
    buffer[4] = (char)('0' + ((int)set_val % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}