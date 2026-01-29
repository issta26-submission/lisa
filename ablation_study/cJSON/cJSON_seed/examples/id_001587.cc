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
//<ID> 1587
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
    cJSON *names_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_array);

    // step 2: Configure
    cJSON_AddItemReferenceToArray(names_array, cJSON_GetArrayItem(names_array, 0));
    cJSON_AddStringToObject(root, "owner", "admin");

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(names_array, 1);
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON_AddBoolToObject(root, "has_names", has_names);
    int remaining = cJSON_GetArraySize(names_array);
    cJSON_AddNumberToObject(root, "count", (double)remaining);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}