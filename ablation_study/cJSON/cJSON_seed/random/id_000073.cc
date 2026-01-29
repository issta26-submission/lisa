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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *names[] = { "alice", "bob", "carol" };
    cJSON *root = cJSON_CreateObject();
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON *active_item = cJSON_CreateTrue();
    cJSON *first_item = (cJSON *)0;
    const char *first_name = (const char *)0;
    cJSON *first_name_copy = (cJSON *)0;

    // step 2: Initialize / Configure - attach array, a reference, a number and a boolean
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON_AddItemReferenceToObject(root, "names_ref", names_arr);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddItemToObject(root, "active", active_item);

    // step 3: Operate - read the first element from the array and copy it into the root
    first_item = cJSON_GetArrayItem(names_arr, 0);
    first_name = cJSON_GetStringValue(first_item);
    first_name_copy = cJSON_CreateString(first_name);
    cJSON_AddItemToObject(root, "first", first_name_copy);

    // step 4: Validate - echo the first name into a secondary entry using an existing API
    cJSON_AddItemToObject(root, "first_duplicate", cJSON_CreateString(cJSON_GetStringValue(cJSON_GetObjectItem(root, "first"))));

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}