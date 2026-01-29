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
//<ID> 1231
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
    cJSON *base_item = NULL;
    cJSON *ref_item = NULL;
    cJSON *num_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_flag = NULL;
    char *printed = NULL;
    double set_result = 0.0;
    cJSON_bool is_false = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    base_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "base", base_item);
    ref_item = cJSON_CreateObjectReference(base_item);
    cJSON_AddItemReferenceToArray(arr, ref_item);
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "num", num_item);
    flag_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate / Validate
    retrieved_num = cJSON_GetObjectItem(root, "num");
    set_result = cJSON_SetNumberHelper(retrieved_num, 42.0);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "arr"));
    printed = cJSON_PrintUnformatted(root);
    (void)set_result;
    (void)is_false;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}