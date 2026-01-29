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
//<ID> 820
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
    cJSON *str = NULL;
    cJSON *raw = NULL;
    cJSON *flag_false = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_data = NULL;
    cJSON *picked_num = NULL;
    cJSON *picked_str = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    double num_value = 0.0;
    cJSON_bool is_string = 0;
    cJSON_bool compare_equal = 0;
    cJSON_bool printed_prealloc = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "data", arr);
    raw = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "meta_raw", raw);
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_prealloc = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    dup_root = cJSON_Duplicate(root, 1);
    dup_data = cJSON_GetObjectItem(dup_root, "data");
    picked_num = cJSON_GetArrayItem(dup_data, 0);
    num_value = cJSON_GetNumberValue(picked_num);
    picked_str = cJSON_GetArrayItem(dup_data, 1);
    is_string = cJSON_IsString(picked_str);
    compare_equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)num_value;
    (void)is_string;
    (void)compare_equal;
    (void)printed_prealloc;
    return 66;
}