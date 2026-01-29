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
    cJSON_AddItemToObject(root, "items", arr);
    const char *version_str = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 2: Configure
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, it1);
    cJSON *it2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, it2);
    cJSON_bool arr_flag = cJSON_IsArray(arr);
    char flagbuf[2];
    flagbuf[0] = (char)('0' + arr_flag);
    flagbuf[1] = '\0';
    cJSON *flag_item = cJSON_CreateString(flagbuf);
    cJSON_AddItemToObject(root, "arr_flag_str", flag_item);

    // step 3: Operate
    const char *spaced_json = "{ \"outer\" : { \"inner\" : [ 1 , 2 , 3 ] } }";
    size_t blen = strlen(spaced_json) + 1;
    char *minbuf = (char *)cJSON_malloc(blen);
    memset(minbuf, 0, blen);
    memcpy(minbuf, spaced_json, blen);
    cJSON_Minify(minbuf);
    cJSON *parsed = cJSON_Parse(minbuf);
    cJSON *outer = cJSON_GetObjectItem(parsed, "outer");
    cJSON *inner = cJSON_GetObjectItem(outer, "inner");
    cJSON_bool inner_is_array = cJSON_IsArray(inner);
    cJSON_AddBoolToObject(root, "inner_is_array", inner_is_array);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    int arr_size = cJSON_GetArraySize(arr);
    char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    (void)first_val;
    (void)arr_size;
    cJSON_free(printed);
    cJSON_free(minbuf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}