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
//<ID> 1583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alice", "bob", "carol" };
    cJSON *array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", array);

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    cJSON_AddItemReferenceToArray(array, first_item);

    // step 3: Operate & Validate
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON_AddNumberToObject(root, "has_names_flag", (double)has_names);
    cJSON_DeleteItemFromArray(array, 1);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}