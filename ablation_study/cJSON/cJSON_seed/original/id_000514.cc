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
//<ID> 514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));
    cJSON *flag_initial = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_initial", flag_initial);

    // step 2: Configure
    int before_count = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int after_count = cJSON_GetArraySize(items);
    cJSON *flag_final = cJSON_AddTrueToObject(root, "flag_final");

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));
    cJSON_bool initial_was_false = cJSON_IsFalse(flag_initial);
    cJSON_bool final_is_true = cJSON_IsTrue(flag_final);

    // step 4: Validate & Cleanup
    (void)before_count;
    (void)after_count;
    (void)first_value;
    (void)initial_was_false;
    (void)final_is_true;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}