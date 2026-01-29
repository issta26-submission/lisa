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
//<ID> 688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(123.0);
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON *num_comp = cJSON_CreateNumber(123.0);

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToArray(array, true_flag);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    int count = cJSON_GetArraySize(array);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    cJSON_bool are_equal = cJSON_Compare(first_item, num_comp, 1);
    (void)version;
    (void)count;
    (void)are_equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(num_comp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}