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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *name_ref = cJSON_CreateStringReference("referenced_string");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = 0;
    cJSON *second_item = NULL;
    double second_value = 0.0;
    double age_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddNumberToObject(root, "age", 30.0);
    cJSON_AddItemToObject(root, "ref", name_ref);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    second_item = cJSON_GetArrayItem(arr, 1);
    second_value = cJSON_GetNumberValue(second_item);
    age_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "age"));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(root, "ref");
    (void)arr_size;
    (void)second_value;
    (void)age_value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}