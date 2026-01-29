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
//<ID> 1580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const names_arr[] = {"Alice", "Bob", "Carol"};
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(names, 0);
    cJSON *second = cJSON_GetArrayItem(names, 1);
    cJSON_AddItemReferenceToArray(refs, first);
    cJSON_AddItemReferenceToArray(refs, second);
    cJSON_DeleteItemFromArray(names, 2);

    // step 3: Operate & Validate
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON_AddNumberToObject(root, "has_names_flag", (double)has_names);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}