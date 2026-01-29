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
//<ID> 1251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"array\":[10,20,30],\"name\":\"initial\"}";
    size_t buf_len = sizeof("{\"array\":[10,20,30],\"name\":\"initial\"}") - 1;
    cJSON *root = cJSON_ParseWithLength(json, buf_len);
    cJSON *array = cJSON_GetObjectItem(root, "array");

    // step 2: Configure
    cJSON *second = cJSON_GetArrayItem(array, 1);
    cJSON *dup_second = cJSON_Duplicate(second, 0);
    cJSON_AddItemToObject(root, "dup_second", dup_second);
    cJSON *s = cJSON_CreateString("created");
    cJSON_SetValuestring(s, "updated");
    cJSON_AddItemToObject(root, "status", s);

    // step 3: Operate and Validate
    cJSON *got_dup = cJSON_GetObjectItem(root, "dup_second");
    cJSON_bool got_is_number = cJSON_IsNumber(got_dup);
    double got_val = cJSON_GetNumberValue(got_dup);
    double computed_value = got_val + (double)(got_is_number);
    cJSON *computed_num = cJSON_CreateNumber(computed_value);
    cJSON_AddItemToObject(root, "computed", computed_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}