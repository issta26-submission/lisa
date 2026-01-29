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
//<ID> 1234
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
    cJSON *num = NULL;
    cJSON *boolean = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_arr = NULL;
    char *printed = NULL;
    double set_result = 0.0;
    double num_value = 0.0;
    cJSON_bool is_false = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(1.0);
    boolean = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "n", num);
    cJSON_AddItemToObject(root, "flag", boolean);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemReferenceToArray(arr, num);

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(num, 3.1415);
    retrieved_num = cJSON_GetObjectItem(root, "n");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    num_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}