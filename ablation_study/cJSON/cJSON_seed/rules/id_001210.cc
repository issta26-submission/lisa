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
//<ID> 1210
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
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *title = NULL;
    cJSON *retrieved = NULL;
    cJSON *first = NULL;
    cJSON *first_copy = NULL;
    char *printed = NULL;
    const char *first_val = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    title = cJSON_CreateString("example_root");
    cJSON_AddItemToObject(root, "title", title);
    item1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, item1);
    item2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item2);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved);
    first = cJSON_GetArrayItem(retrieved, 0);
    first_val = cJSON_GetStringValue(first);
    first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    printed = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)first_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}