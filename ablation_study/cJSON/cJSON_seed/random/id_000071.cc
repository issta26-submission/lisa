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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *names[] = { "Alice", "Bob", "Carol" };
    int names_count = 3;
    cJSON *root = cJSON_CreateObject();
    cJSON *names_arr = cJSON_CreateStringArray(names, names_count);
    cJSON *count_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    const char *first_val = (const char *)0;

    // step 2: Initialize - attach array and add numeric metadata
    cJSON_AddItemToObject(root, "names", names_arr);
    count_item = cJSON_AddNumberToObject(root, "count", (double)names_count);

    // step 3: Configure - add a true flag and a reference to the names array
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);
    cJSON_AddItemReferenceToObject(root, "names_ref", names_arr);

    // step 4: Operate - read first element from the attached array
    retrieved = cJSON_GetObjectItem(root, "names");
    first = cJSON_GetArrayItem(retrieved, 0);
    first_val = cJSON_GetStringValue(first);

    // step 5: Validate - store the retrieved string as a new named item
    cJSON_AddItemToObject(root, "first_name", cJSON_CreateString(first_val));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}