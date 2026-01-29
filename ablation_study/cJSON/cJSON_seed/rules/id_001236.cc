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
//<ID> 1236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *cfg = NULL;
    cJSON *num_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *arr = NULL;
    cJSON *retrieved_cfg = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_bool = NULL;
    cJSON *retrieved_refs = NULL;
    char *printed = NULL;
    double old_val = 0.0;
    double set_result = 0.0;
    double delta = 0.0;
    cJSON_bool is_false_result = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cfg = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14);
    bool_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(cfg, "pi", num_item);
    cJSON_AddItemToObject(cfg, "enabled", bool_item);
    cJSON_AddItemToObject(root, "config", cfg);
    arr = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(arr, num_item);
    cJSON_AddItemReferenceToArray(arr, bool_item);
    cJSON_AddItemToObject(root, "refs", arr);

    // step 3: Operate / Validate
    retrieved_cfg = cJSON_GetObjectItem(root, "config");
    retrieved_num = cJSON_GetObjectItem(retrieved_cfg, "pi");
    old_val = cJSON_GetNumberValue(retrieved_num);
    set_result = cJSON_SetNumberHelper(retrieved_num, 6.28);
    delta = set_result - old_val;
    retrieved_bool = cJSON_GetObjectItem(retrieved_cfg, "enabled");
    is_false_result = cJSON_IsFalse(retrieved_bool);
    retrieved_refs = cJSON_GetObjectItem(root, "refs");
    arr_size = cJSON_GetArraySize(retrieved_refs);
    cJSON_AddNumberToObject(root, "delta", delta);
    cJSON_AddNumberToObject(root, "ref_count", (double)arr_size);
    cJSON_AddBoolToObject(root, "was_false", is_false_result);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}