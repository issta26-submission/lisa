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
//<ID> 869
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
    cJSON *str_ref = NULL;
    char *printed = NULL;
    const char *arr_str = NULL;
    double pi_val = 0.0;
    double first_num = 0.0;
    int arr_size = 0;
    cJSON_bool is_str = 0;
    cJSON_bool is_num = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(arr, 1, num2);
    str_ref = cJSON_CreateStringReference("hello_reference");
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);
    is_str = cJSON_IsString(cJSON_GetArrayItem(arr, 2));
    is_num = cJSON_IsNumber(cJSON_GetArrayItem(arr, 0));
    arr_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 2));
    first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    buffer[0] = (is_str ? '1' : '0');
    buffer[1] = (is_num ? '1' : '0');
    (void)arr_size;
    (void)arr_str;
    (void)first_num;
    (void)pi_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}