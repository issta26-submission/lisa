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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 2: Configure
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *dup_meta = cJSON_Duplicate(got_meta, 1);
    cJSON *dup_name = cJSON_GetObjectItem(dup_meta, "name");
    char *set_ret = cJSON_SetValuestring(dup_name, "updated_backup");
    (void)set_ret;
    cJSON_AddItemToObject(root, "meta_backup", dup_meta);

    // step 3: Operate and Validate
    int arr_sz = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "array_size", (double)arr_sz);
    cJSON *orig_name = cJSON_GetObjectItem(meta, "name");
    const char *name_str = cJSON_GetStringValue(orig_name);
    double first_char_code = (double)(unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "name_first_char_code", first_char_code);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}