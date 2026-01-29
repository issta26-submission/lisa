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
//<ID> 258
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
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON *count_item = NULL;
    cJSON *sref = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = 0;
    double first_element_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    count_item = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    sref = cJSON_CreateStringReference("example_label");
    cJSON_AddItemToObject(root, "label", sref);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "label");
    first_element_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)first_element_value;
    (void)count_item;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}