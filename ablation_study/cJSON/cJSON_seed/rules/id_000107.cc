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
//<ID> 107
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
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *meta = cJSON_CreateObject();
    char *buffer = NULL;
    cJSON_bool printed_ok = 0;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_name = NULL;
    cJSON_bool name_is_string = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(root, "name", "container");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 4.0);

    // step 3: Operate / Validate
    buffer = (char*)cJSON_malloc((size_t)512);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 512, 0);
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_is_string = cJSON_IsString(retrieved_name);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_string);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}