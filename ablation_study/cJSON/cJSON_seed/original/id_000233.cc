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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("foo");
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, btrue);

    // step 2: Configure
    cJSON_bool is_b = cJSON_IsBool(btrue);

    // step 3: Operate and Validate
    int before = cJSON_GetArraySize(items);
    cJSON_DeleteItemFromArray(items, 1);
    int after = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *printed = cJSON_Print(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = before + after + (int)first_val + (is_b ? 1 : 0) + (printed ? (int)printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}