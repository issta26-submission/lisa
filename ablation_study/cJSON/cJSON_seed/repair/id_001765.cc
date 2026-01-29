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
//<ID> 1765
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(values, 1, n2);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n3);

    // step 2: Configure
    double updated_n2 = cJSON_SetNumberHelper(n2, 25.5);
    cJSON *new_n3 = cJSON_CreateNumber(35.0);
    cJSON_ReplaceItemViaPointer(values, n3, new_n3);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("unit-test"));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    int array_size = cJSON_GetArraySize(values);
    double v1 = cJSON_GetNumberValue(n1);
    double v2 = updated_n2;
    double v3 = cJSON_GetNumberValue(new_n3);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (array_size <= 9 ? array_size : 9));
    buffer[2] = (char)('0' + (int)((int)v1 % 10));
    buffer[3] = (char)('0' + (int)((int)v2 % 10));
    buffer[4] = (char)('0' + (int)((int)v3 % 10));
    (void)buffer;
    (void)v1;
    (void)v2;
    (void)v3;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}