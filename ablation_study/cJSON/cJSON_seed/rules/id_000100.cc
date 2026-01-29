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
//<ID> 100
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
    cJSON *s2 = cJSON_CreateString("Beta");
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *label_item = NULL;
    cJSON *label_item_cs = NULL;
    char *printed = NULL;
    void *scratch = NULL;
    int arr_size = 0;
    cJSON_bool is_string1 = 0;
    cJSON_bool is_string_cs = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "label", "Example");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    printed = cJSON_PrintUnformatted(root);
    scratch = cJSON_malloc((size_t)32);
    memset(scratch, 0, (size_t)32);
    label_item = cJSON_GetObjectItem(root, "label");
    is_string1 = cJSON_IsString(label_item);
    label_item_cs = cJSON_GetObjectItemCaseSensitive(root, "label");
    is_string_cs = cJSON_IsString(label_item_cs);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}