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
//<ID> 884
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
    int size_before = 0;
    int size_after = 0;
    double first_val = 0.0;
    double sum_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddNumberToObject(root, "sum", 30.75);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_before = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    sum_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "sum"));
    cJSON_DeleteItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);
    (void)size_before;
    (void)size_after;
    (void)first_val;
    (void)sum_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}