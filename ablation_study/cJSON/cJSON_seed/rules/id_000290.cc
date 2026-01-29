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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"a\": 42.5, \"b\": [true, false], \"str\":\"hello\"}";
    size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(0.0);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":123}");
    char *printed = NULL;
    double a_value = 0.0;
    double set_result = 0.0;
    cJSON *a_item = NULL;
    const char *src_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToObject(root, "raw", raw);
    a_item = cJSON_GetObjectItem(parsed, "a");
    a_value = cJSON_GetNumberValue(a_item);
    set_result = cJSON_SetNumberHelper(num0, a_value + 2.5);
    src_str = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "str"));
    cJSON_AddStringToObject(root, "source_str", src_str);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    (void)set_result;
    (void)parse_end;
    (void)src_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}