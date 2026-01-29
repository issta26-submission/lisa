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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = { "one", "two", "three" };
    cJSON *str_arr = cJSON_CreateStringArray(strings, 3);
    float floats[] = { 1.1f, 2.2f, 3.3f };
    cJSON *float_arr = cJSON_CreateFloatArray(floats, 3);
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON_AddItemToObject(root, "floats", float_arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "creator", "synth");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *num = cJSON_CreateNumber(4.2);
    cJSON_AddItemToObject(meta, "version", num);
    cJSON_bool are_equal = cJSON_Compare(root, dup_root, 1);
    int summary = (printed ? (int)printed[0] : 0) + (int)are_equal + cJSON_GetArraySize(str_arr);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}