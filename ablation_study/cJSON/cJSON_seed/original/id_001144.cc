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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *items[] = { "alpha", "beta", "gamma" };
    cJSON *str_arr = cJSON_CreateStringArray(items, 3);
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "nested_num", 3.14159);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemReferenceToObject(root, "nested_ref", nested);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(84.0);
    cJSON_bool did_replace = cJSON_ReplaceItemViaPointer(root, num_item, replacement);
    (void)did_replace;
    cJSON_DeleteItemFromObject(root, "strings");

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}