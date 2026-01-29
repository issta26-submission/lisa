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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("alpha");
    const char *ref_text = "beta_ref";
    cJSON *sref = cJSON_CreateStringReference(ref_text);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, sref);

    // step 2: Configure
    cJSON *s2 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    const char *second_val = cJSON_GetStringValue(second);
    char buf[128];
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|%s", first_val, second_val);
    cJSON *combined = cJSON_CreateString(buf);
    cJSON_ReplaceItemInArray(arr, 1, combined);
    cJSON *new_second = cJSON_GetArrayItem(arr, 1);
    const char *new_second_val = cJSON_GetStringValue(new_second);
    (void)new_second_val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}