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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);
    cJSON_AddItemToObject(root, "numbers", array);

    // step 2: Configure
    cJSON *ref_item = cJSON_CreateString("shared");
    cJSON_AddItemReferenceToArray(array, ref_item);
    cJSON_bool array_flag = cJSON_IsArray(array);
    cJSON_AddBoolToObject(root, "numbers_is_array", array_flag);
    cJSON *to_replace = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "placeholder", to_replace);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(100.0);
    cJSON_ReplaceItemViaPointer(root, to_replace, replacement);
    cJSON_AddNullToObject(root, "none");
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(unformatted);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}