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
//<ID> 1065
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON *active = cJSON_AddTrueToObject(root, "active");
    (void)active;
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, flag);

    // step 2: Configure
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddItemToArray(arr, entry);
    cJSON_AddStringToObject(entry, "name", "entry1");
    cJSON_AddNumberToObject(entry, "value", 3.14);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Operate and Validate
    cJSON_bool nested_is_obj = cJSON_IsObject(nested);
    int arr_size = cJSON_GetArraySize(arr);
    char *flat = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (arr_size & 0xF));
    ((char *)scratch)[1] = (char)('0' + nested_is_obj);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}