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
//<ID> 1110
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON *name = cJSON_CreateString("orig");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, flag);

    // step 2: Configure
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *newname = cJSON_SetValuestring(nameItem, "updated");
    cJSON_AddStringToObject(root, "note", "metadata");

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool is_bool = cJSON_IsBool(first);
    cJSON_bool is_str = cJSON_IsString(nameItem);
    double computed = 10.0 + (double)is_bool * 2.0 + (double)is_str * 3.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = newname[0];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}