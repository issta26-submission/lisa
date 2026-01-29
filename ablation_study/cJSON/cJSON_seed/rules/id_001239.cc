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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"config\":{\"value\":1},\"flags\":{\"enabled\":false}}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *cfg = NULL;
    cJSON *val = NULL;
    cJSON *flags = NULL;
    cJSON *enabled = NULL;
    cJSON *arr = NULL;
    cJSON *num_ref = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    double set_result = 0.0;
    cJSON_bool is_false_flag = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    cfg = cJSON_GetObjectItem(root, "config");
    val = cJSON_GetObjectItem(cfg, "value");
    flags = cJSON_GetObjectItem(root, "flags");
    enabled = cJSON_GetObjectItem(flags, "enabled");

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(val, 42.5);
    arr = cJSON_CreateArray();
    num_ref = cJSON_CreateNumber(7);
    cJSON_AddItemToArray(arr, num_ref);
    cJSON_AddItemToObject(root, "items", arr);
    retrieved = cJSON_GetObjectItem(root, "items");
    is_false_flag = cJSON_IsFalse(enabled);
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(retrieved);
    (void)set_result;
    (void)is_false_flag;
    (void)parse_end;
    (void)arr_size;
    (void)json_len;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}