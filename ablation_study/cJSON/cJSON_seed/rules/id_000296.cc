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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawKey\":true}");
    cJSON *arr = cJSON_CreateArray();
    cJSON *s = cJSON_CreateString("hello");
    char *printed = NULL;
    char *prealloc = (char *)cJSON_malloc(128);
    memset(prealloc, 0, 128);
    const char *parse_end = NULL;
    cJSON *parsed = NULL;
    double set_result = 0.0;
    int arr_size = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "name", s);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.5));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));

    // step 3: Operate / Validate
    set_result = cJSON_SetNumberHelper(num_item, 9.81);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed), &parse_end, 1);
    arr_size = cJSON_GetArraySize(arr);
    printed_ok = cJSON_PrintPreallocated(root, prealloc, 128, 0);
    prealloc[0] = (char)('0' + (arr_size % 10));
    (void)set_result;
    (void)printed_ok;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(prealloc);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}