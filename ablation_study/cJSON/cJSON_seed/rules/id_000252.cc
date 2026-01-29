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
//<ID> 252
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
    const char *ref_str = "hello_ref";
    cJSON *sref = cJSON_CreateStringReference(ref_str);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = 0;
    double first_value = 0.0;
    double second_value = 0.0;
    double computed_sum = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "initial_count", (double)arr_size);

    // step 3: Operate / Validate
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    computed_sum = first_value + second_value;
    cJSON_AddNumberToObject(root, "computed_sum", computed_sum);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "initial_count");
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)first_value;
    (void)second_value;
    (void)computed_sum;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}