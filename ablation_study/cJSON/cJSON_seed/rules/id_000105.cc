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
//<ID> 105
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
    cJSON *n1 = cJSON_CreateNumber(7.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON *first_item = NULL;
    cJSON *items_via_get = NULL;
    cJSON *items_via_get_cs = NULL;
    cJSON *first_ref = NULL;
    cJSON_bool is_first_string = 0;
    cJSON_bool is_ref_string = 0;
    cJSON_bool printed_ok = 0;
    char *buffer = (char *)cJSON_malloc(256);

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "first", cJSON_CreateStringReference("alpha"));

    // step 3: Operate / Validate
    items_via_get = cJSON_GetObjectItem(root, "items");
    items_via_get_cs = cJSON_GetObjectItemCaseSensitive(root, "items");
    first_item = cJSON_GetArrayItem(items_via_get_cs, 0);
    first_ref = cJSON_GetObjectItemCaseSensitive(meta, "first");
    is_first_string = cJSON_IsString(first_item);
    is_ref_string = cJSON_IsString(first_ref);
    memset(buffer, 0, 256);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}