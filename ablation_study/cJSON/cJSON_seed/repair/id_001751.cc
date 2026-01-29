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
//<ID> 1751
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateString("end"));

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("sample"));
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    char *printed = cJSON_Print(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = cJSON_GetArraySize(values);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_num = cJSON_GetNumberValue(first_item);
    cJSON *third_item = cJSON_GetArrayItem(values, 2);
    const char *third_str = cJSON_GetStringValue(third_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)equal);
    buffer[2] = (char)('0' + (int)arr_size);
    buffer[3] = (char)('0' + ((int)first_num % 10));
    buffer[4] = third_str ? third_str[0] : 'Z';
    (void)buffer;
    (void)first_num;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}