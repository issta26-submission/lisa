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
//<ID> 986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *arr_item1 = NULL;
    cJSON *false_item = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_arr = NULL;
    cJSON *parsed_second = NULL;
    char *printed = NULL;
    char *unformatted = NULL;
    char *preallocated = NULL;
    char *minify_buf = NULL;
    const char json_text[] = "{ \"a\": [1, 2, 3], \"b\": false }";
    size_t json_len = (size_t)(sizeof(json_text) - 1);
    double extracted_value = 0.0;
    int parsed_b_is_false = 0;
    int pre_len = 128;

    // step 2: Setup / Configure
    minify_buf = (char *)cJSON_malloc(json_len + 1);
    memcpy(minify_buf, json_text, json_len);
    minify_buf[json_len] = '\0';
    cJSON_Minify(minify_buf);
    parsed = cJSON_Parse(minify_buf);

    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    arr_item0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(arr, arr_item0);
    arr_item1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, arr_item1);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", false_item);

    // step 3: Operate / Validate
    unformatted = cJSON_PrintUnformatted(root);
    printed = cJSON_Print(root);
    preallocated = (char *)cJSON_malloc((size_t)pre_len);
    memset(preallocated, 0, (size_t)pre_len);
    cJSON_PrintPreallocated(root, preallocated, pre_len, 0);

    parsed_arr = cJSON_GetObjectItem(parsed, "a");
    parsed_second = cJSON_GetArrayItem(parsed_arr, 1);
    extracted_value = cJSON_GetNumberValue(parsed_second);
    parsed_b_is_false = (int)cJSON_IsFalse(cJSON_GetObjectItem(parsed, "b"));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(unformatted);
    cJSON_free(preallocated);
    cJSON_free(minify_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}