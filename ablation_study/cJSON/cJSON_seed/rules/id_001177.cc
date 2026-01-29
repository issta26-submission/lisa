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
//<ID> 1177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *names_array = NULL;
    cJSON *arr2 = NULL;
    cJSON *transient_str = NULL;
    cJSON *first_item = NULL;
    cJSON *copied_first = NULL;
    cJSON *num_item = NULL;
    char *printed = NULL;
    const char *names[] = { "Alice", "Bob", "Charlie" };

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    names_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_array);
    arr2 = cJSON_CreateArray();
    transient_str = cJSON_CreateString("temporary");
    cJSON_AddItemToArray(arr2, transient_str);
    cJSON_AddItemToObject(root, "transients", arr2);
    num_item = cJSON_CreateNumber(1.0);
    cJSON_SetNumberHelper(num_item, 42.0);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 3: Operate / Validate
    first_item = cJSON_GetArrayItem(names_array, 0);
    copied_first = cJSON_CreateString(cJSON_GetStringValue(first_item));
    cJSON_AddItemToObject(root, "first_copy", copied_first);
    cJSON_DeleteItemFromArray(arr2, 0);
    printed = cJSON_PrintUnformatted(root);
    (void)cJSON_GetStringValue(copied_first);
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}