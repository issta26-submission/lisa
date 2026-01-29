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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char js[] = "{\"alpha\": 1, \"list\": [2, 3]} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(js, sizeof(js) - 1, &parse_end, 0);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *raw = cJSON_CreateRaw("{\"rawKey\": \"rawVal\"}");
    char *printed = NULL;
    double set_result = 0.0;
    int arr_size = 0;
    double first_num = 0.0;
    size_t consumed = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));
    cJSON_AddItemToObject(root, "num", num);
    set_result = cJSON_SetNumberHelper(num, 3.14159);
    cJSON_AddItemToObject(root, "raw_fragment", raw);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    arr_size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    first_num = cJSON_GetNumberValue(first_item);
    consumed = (size_t)(parse_end - js);
    (void)consumed;
    (void)set_result;
    (void)arr_size;
    (void)first_num;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}