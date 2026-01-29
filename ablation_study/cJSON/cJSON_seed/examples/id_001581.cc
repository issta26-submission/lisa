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
//<ID> 1581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "Alice", "Bob", "Carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    cJSON *ref_dave = cJSON_CreateString("Dave");
    cJSON_bool added_ref1 = cJSON_AddItemReferenceToArray(names, ref_dave);
    cJSON *ref_eve = cJSON_CreateString("Eve");
    cJSON_bool added_ref2 = cJSON_AddItemReferenceToArray(names, ref_eve);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(names, 1);
    int remaining = cJSON_GetArraySize(names);
    cJSON *first_item = cJSON_GetArrayItem(names, 0);
    const char *first_name = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(root, "first_name", first_name);
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON_AddNumberToObject(root, "has_names_flag", (double)has_names);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}