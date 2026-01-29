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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateArray();
    cJSON *b_true = cJSON_CreateBool(1);
    cJSON *b_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(child, b_true);
    cJSON_AddItemToArray(child, b_false);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    // Replace a direct reference (which could cause double-free hazards)
    // with a deep duplicate that preserves the intended logical result.
    cJSON *arr_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_ref", arr_dup);
    char *tmp_buf = (char *)cJSON_malloc(16);
    memset(tmp_buf, 0, 16);
    tmp_buf[0] = 'v';
    tmp_buf[1] = '1';
    tmp_buf[2] = '\0';

    // step 3: Operate and Validate
    int size_before = cJSON_GetArraySize(child);
    cJSON *first_item = cJSON_GetArrayItem(child, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_item);
    char *printed = cJSON_PrintUnformatted(root);
    int summary = size_before + (int)first_is_true + (printed ? (int)printed[0] : 0) + (int)tmp_buf[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(tmp_buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    return 66;
}