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
//<ID> 668
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *manual = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "manual_items", manual);
    cJSON *auto_arr = cJSON_AddArrayToObject(root, "auto_items");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(manual, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(auto_arr, s2);
    cJSON *name = cJSON_CreateString("demo");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "manual_count", (double)cJSON_GetArraySize(manual));
    cJSON_AddNumberToObject(root, "auto_count", (double)cJSON_GetArraySize(auto_arr));
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(manual, 0);
    cJSON_bool first_is_str = cJSON_IsString(first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_str);
    int buflen = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_AddBoolToObject(root, "printed_preallocated", printed);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}