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
//<ID> 1138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *true_created = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_created);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "flag");
    char *set_ret = cJSON_SetValuestring(str_item, "updated");
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double num = cJSON_GetNumberValue(second);
    double computed_val = num + (double)size;
    cJSON *computed_item = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = set_ret ? set_ret[0] : 0;
    ((char *)scratch)[3] = set_ret ? set_ret[1] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}