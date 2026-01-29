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
//<ID> 1330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *placeholder = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(meta, "placeholder", placeholder);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(meta, placeholder, replacement);

    // step 3: Operate and Validate
    int arr_size = cJSON_GetArraySize(numbers);
    cJSON *first_item = cJSON_GetArrayItem(numbers, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    double repl_val = cJSON_GetNumberValue(replacement);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (arr_size % 10));
    ((char *)scratch)[2] = (char)('0' + (((int)first_val) % 10 + 10) % 10);
    ((char *)scratch)[3] = (char)('0' + (((int)repl_val) % 10 + 10) % 10);
    ((char *)scratch)[4] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}