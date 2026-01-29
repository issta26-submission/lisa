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
//<ID> 1221
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    const char *ver = cJSON_Version();
    int raw_buf_len = 128;
    char *raw = (char *)cJSON_malloc((size_t)raw_buf_len);
    memset(raw, 0, (size_t)raw_buf_len);
    const char *raw_src = " { \"num\" : 123 , \"arr\" : [ 1 , 2 , 3 ] } ";
    strcpy(raw, raw_src);
    cJSON_Minify(raw);
    cJSON_bool is_array = cJSON_IsArray(arr);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(raw);
    cJSON_AddItemToObject(root, "parsed", parsed);
    int arr_size = cJSON_GetArraySize(arr);
    const char *second = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));

    // step 4: Validate & Cleanup
    (void)ver;
    (void)is_array;
    (void)arr_size;
    (void)second;
    cJSON_free(printed);
    cJSON_free(raw);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}