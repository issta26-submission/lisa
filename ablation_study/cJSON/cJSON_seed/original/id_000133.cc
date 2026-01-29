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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    const char *string_list[] = { "one", "two", "three" };
    cJSON *sarr = cJSON_CreateStringArray(string_list, 3);
    cJSON_AddItemToObject(root, "strings", sarr);
    float float_list[] = { 1.1f, 2.2f, 3.3f };
    cJSON *farr = cJSON_CreateFloatArray(float_list, 3);
    cJSON_AddItemToObject(root, "numbers", farr);

    // step 2: Configure
    cJSON *dup_sarr = cJSON_Duplicate(sarr, 1);
    cJSON_AddItemToObject(root, "strings_copy", dup_sarr);
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", dup_meta);
    cJSON *detached_nums = cJSON_DetachItemFromObject(root, "numbers");
    cJSON_AddItemToObject(meta, "mynums", detached_nums);

    // step 3: Operate and Validate
    cJSON_bool arrays_equal = cJSON_Compare(sarr, dup_sarr, 1);
    int strings_size = cJSON_GetArraySize(sarr);
    cJSON *first_num_item = cJSON_GetArrayItem(farr, 0);
    double first_num = cJSON_GetNumberValue(first_num_item);
    char *printed = cJSON_PrintUnformatted(root);
    int summary = (int)arrays_equal + strings_size + (int)first_num + (printed ? (int)printed[0] : 0);
    (void)summary;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}