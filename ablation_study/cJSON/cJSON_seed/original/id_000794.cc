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
//<ID> 794
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"initial\":\"data\",\"arr\":[10,20]}";
    size_t json_len = (sizeof(json_text) - 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *aux = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *num_item = cJSON_CreateNumber(7.5);
    cJSON *str_item = cJSON_CreateString("sample");

    // step 2: Configure
    cJSON_AddItemToArray(array, num_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToArray(array, raw_item);
    cJSON_AddItemToArray(array, null_item);
    cJSON_AddItemToObject(root, "added", array);
    cJSON_AddItemToObject(aux, "copied", cJSON_Duplicate(array, 1));

    // step 3: Operate
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON_bool was_raw = cJSON_IsRaw(raw_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool equal_roots = cJSON_Compare(root, dup_root, 1);
    char *out_root = cJSON_PrintUnformatted(root);
    char *out_dup = cJSON_PrintUnformatted(dup_root);
    cJSON_free(out_root);
    cJSON_free(out_dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(aux);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}