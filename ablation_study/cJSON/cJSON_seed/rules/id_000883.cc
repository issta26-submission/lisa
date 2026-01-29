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
//<ID> 883
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    char *printed = NULL;
    double first_val = 0.0;
    double second_val = 0.0;
    int arr_size = 0;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddNumberToObject(root, "extra", 42.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('0' + (arr_size & 0xF));
    (void)first_val;
    (void)second_val;
    (void)printed;

    // step 4: Cleanup
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}