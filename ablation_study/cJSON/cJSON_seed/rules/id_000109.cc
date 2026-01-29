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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *label = cJSON_CreateString("rootLabel");
    cJSON *items_ptr = NULL;
    cJSON *label_ptr = NULL;
    char *printed = NULL;
    void *tmp_mem = NULL;
    int count = 0;
    cJSON_bool is_label_string = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", label);
    tmp_mem = cJSON_malloc((size_t)32);

    // step 3: Operate / Validate
    items_ptr = cJSON_GetObjectItem(root, "items");
    label_ptr = cJSON_GetObjectItemCaseSensitive(root, "label");
    is_label_string = cJSON_IsString(label_ptr);
    count = cJSON_GetArraySize(items_ptr);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddBoolToObject(root, "label_is_string", is_label_string);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}