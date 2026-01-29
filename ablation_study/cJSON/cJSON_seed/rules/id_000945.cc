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
//<ID> 945
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *elem_a = NULL;
    cJSON *elem_b = NULL;
    cJSON *copied_elem = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    char *printed2 = NULL;
    const char *text_a = "alpha";
    const char *text_b = "beta";
    const char *retrieved_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("root_object"));
    array = cJSON_CreateArray();
    elem_a = cJSON_CreateString(text_a);
    elem_b = cJSON_CreateString(text_b);
    cJSON_AddItemToArray(array, elem_a);
    cJSON_AddItemToArray(array, elem_b);
    cJSON_AddItemToObjectCS(root, "items", array);
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("generated_by_test"));

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    retrieved = cJSON_GetArrayItem(array, 1);
    retrieved_str = cJSON_GetStringValue(retrieved);
    copied_elem = cJSON_CreateString(retrieved_str);
    cJSON_AddItemToObject(root, "second_item_copy", copied_elem);
    printed2 = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}