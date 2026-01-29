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
//<ID> 1228
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *base_num = cJSON_CreateNumber(3.5);
    cJSON_AddItemToObject(root, "base", base_num);

    // step 2: Configure
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s2);
    cJSON_bool s1_is_string = cJSON_IsString(s1);
    double base_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "base"));
    double combined = base_val + (double)s1_is_string;
    cJSON *final_num = cJSON_CreateNumber(combined);
    cJSON_AddItemToObject(root, "final", final_num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = strlen(out);
    char *copy = (char *)cJSON_malloc(out_len + 1);
    memset(copy, 0, out_len + 1);
    memcpy(copy, out, out_len + 1);
    cJSON_Minify(copy);
    cJSON *parsed = cJSON_Parse(copy);
    cJSON *parsed_final = cJSON_GetObjectItem(parsed, "final");
    double parsed_val = cJSON_GetNumberValue(parsed_final);
    double extra = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "base"));
    double arr_second_is_string = (double)cJSON_IsString(cJSON_GetArrayItem(arr, 1));
    double total = parsed_val + extra + arr_second_is_string;
    char *scratch = (char *)cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((unsigned char *)scratch)[0] = (unsigned char)((int)total & 0xFF);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(copy);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}