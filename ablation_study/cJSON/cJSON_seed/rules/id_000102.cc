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
//<ID> 102
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
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *name_item = cJSON_CreateString("example");
    char *buffer = (char *)cJSON_malloc((size_t)256);
    memset(buffer, 0, 256);
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    cJSON_bool printed_ok = 0;
    cJSON *got_name_via_get = NULL;
    cJSON *got_name_via_case = NULL;
    cJSON *got_items = NULL;
    cJSON_bool name_is_string = 0;
    int items_count = 0;

    // step 2: Setup / Configure structure
    added_num = cJSON_AddItemToArray(arr, num_item);
    added_str = cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate / Validate
    got_name_via_get = cJSON_GetObjectItem(root, "name");
    got_name_via_case = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_is_string = cJSON_IsString(got_name_via_case);
    got_items = cJSON_GetObjectItem(root, "items");
    items_count = cJSON_GetArraySize(got_items);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}