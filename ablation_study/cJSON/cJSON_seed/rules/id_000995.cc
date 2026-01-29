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
//<ID> 995
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
    cJSON *is_arr_bool = NULL;
    char *buffer = NULL;
    cJSON_bool is_array_flag = 0;
    int prebuffer = 128;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    num0 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, num0);
    num1 = cJSON_CreateNumber(3.14);
    cJSON_InsertItemInArray(arr, 1, num1);

    // step 3: Operate / Validate
    is_array_flag = cJSON_IsArray(arr);
    is_arr_bool = cJSON_CreateBool(is_array_flag);
    cJSON_AddItemToObject(root, "data_is_array", is_arr_bool);
    buffer = cJSON_PrintBuffered(root, prebuffer, 1);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}