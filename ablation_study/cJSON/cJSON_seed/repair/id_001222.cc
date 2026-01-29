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
//<ID> 1222
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
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *name = cJSON_CreateString("fuzz-name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(6.5);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("example-label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(meta, "extras", extras);
    cJSON *exnum = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(extras, exnum);

    // step 3: Operate and Validate
    cJSON_bool is_name_string = cJSON_IsString(name);
    cJSON_bool is_label_string = cJSON_IsString(label);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double vex = cJSON_GetNumberValue(cJSON_GetArrayItem(extras, 0));
    double total = v0 + v1 + vex + (double)is_name_string + (double)is_label_string;
    cJSON_AddNumberToObject(root, "final", total);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_SetValuestring(name, scratch);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}