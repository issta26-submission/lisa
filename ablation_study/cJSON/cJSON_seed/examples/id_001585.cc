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
//<ID> 1585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const names_list[3] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *names_array = cJSON_CreateStringArray(names_list, 3);
    cJSON_AddItemToObject(root, "names", names_array);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(names_array, 0);
    cJSON_AddItemReferenceToArray(refs, first_item);
    cJSON *second_item = cJSON_GetArrayItem(names_array, 1);
    cJSON *second_dup = cJSON_Duplicate(second_item, 1);
    cJSON_AddItemToArray(refs, second_dup);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(names_array, 2);
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    int names_count = cJSON_GetArraySize(names_array);
    int refs_count = cJSON_GetArraySize(refs);
    char *printed = cJSON_PrintUnformatted(root);
    (void)has_names;
    (void)names_count;
    (void)refs_count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}