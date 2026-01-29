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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *name = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *name_copy = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    const char *retrieved_text = NULL;
    cJSON_bool name_is_string = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "name");
    name_is_string = cJSON_IsString(retrieved);
    retrieved_text = cJSON_GetStringValue(retrieved);
    name_copy = cJSON_CreateString(retrieved_text);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    printed = cJSON_PrintUnformatted(root);
    (void)name_is_string;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}