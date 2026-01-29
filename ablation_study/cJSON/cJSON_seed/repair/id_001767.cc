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
//<ID> 1767
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
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, num0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, num1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "env", "integration");
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // step 3: Operate and Validate
    double updated = cJSON_SetNumberHelper(num0, 11.5);
    cJSON *mid = cJSON_CreateNumber(15.0);
    cJSON_InsertItemInArray(values, 1, mid);
    cJSON *replacement = cJSON_CreateNumber(25.0);
    cJSON_ReplaceItemViaPointer(values, num1, replacement);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    double rep_val = cJSON_GetNumberValue(replacement);
    int size = cJSON_GetArraySize(values);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)size);
    buffer[2] = (char)('0' + ((int)updated % 10));
    buffer[3] = (char)('0' + ((int)rep_val % 10));
    (void)buffer;
    (void)printed;
    (void)updated;
    (void)rep_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}