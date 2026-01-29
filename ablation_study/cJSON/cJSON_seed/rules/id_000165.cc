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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON *arr = cJSON_CreateArray();
    cJSON *arr_item1 = cJSON_CreateNumber(1.0);
    cJSON *arr_item2 = cJSON_CreateNumber(2.0);
    int arr_size = 0;
    double retrieved_number = 0.0;
    double first_array_value = 0.0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddItemToArray(arr, arr_item1);
    cJSON_AddItemToArray(arr, arr_item2);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    retrieved_number = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    first_array_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    snprintf(buffer, sizeof(buffer), "number=%.2f list_size=%d first=%.2f", retrieved_number, arr_size, first_array_value);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)buffer;
    return 66;
}