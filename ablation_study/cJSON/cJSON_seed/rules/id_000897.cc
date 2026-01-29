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
//<ID> 897
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *newnum = NULL;
    cJSON *retrieved_numbers = NULL;
    cJSON *retrieved_scalar = NULL;
    char *printed = NULL;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));
    double val0 = 0.0;
    double val1 = 0.0;
    double scalar = 0.0;
    int sum_int = 0;
    cJSON_bool first_was_number = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num0 = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num0);
    num1 = cJSON_CreateNumber(20.75);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddNumberToObject(root, "scalar", 3.0);

    // step 3: Operate / Validate
    retrieved_numbers = cJSON_GetObjectItem(root, "numbers");
    first_was_number = cJSON_IsNumber(cJSON_GetArrayItem(retrieved_numbers, 0));
    newnum = cJSON_CreateNumber(99.9);
    replaced = cJSON_ReplaceItemInArray(retrieved_numbers, 0, newnum);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_numbers, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_numbers, 1));
    retrieved_scalar = cJSON_GetObjectItem(root, "scalar");
    scalar = cJSON_GetNumberValue(retrieved_scalar);
    sum_int = (int)(val0 + val1 + scalar);
    scratch[0] = (char)('0' + (sum_int % 10));
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)first_was_number;
    (void)replaced;
    (void)scratch;
    (void)val0;
    (void)val1;
    (void)scalar;
    (void)sum_int;
    return 66;
}