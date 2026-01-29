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
//<ID> 862
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
    cJSON *str_ref = NULL;
    cJSON *num_item = NULL;
    cJSON *added_number = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    char *printed = NULL;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool insert_res0 = 0;
    cJSON_bool insert_res1 = 0;
    int arr_size = 0;
    const char *str0 = NULL;
    double val1 = 0.0;
    cJSON_bool is_str0 = 0;
    cJSON_bool is_num1 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    str_ref = cJSON_CreateStringReference("hello_ref");
    insert_res0 = cJSON_InsertItemInArray(arr, 0, str_ref);
    num_item = cJSON_CreateNumber(42.5);
    insert_res1 = cJSON_InsertItemInArray(arr, 1, num_item);
    added_number = cJSON_AddNumberToObject(root, "sum", 84.5);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    str0 = cJSON_GetStringValue(got0);
    val1 = cJSON_GetNumberValue(got1);
    is_str0 = cJSON_IsString(got0);
    is_num1 = cJSON_IsNumber(got1);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)insert_res0;
    (void)insert_res1;
    (void)added_number;
    (void)str0;
    (void)is_str0;
    (void)is_num1;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}