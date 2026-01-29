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
//<ID> 658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *json_copy = (char *)cJSON_malloc(512);
    char *scratch = (char *)cJSON_malloc(512);
    memset(json_copy, 0, 512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *sub = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON *array_item = NULL;
    cJSON_bool has_sub = 0;
    cJSON_bool raw_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "nullval", null_item);
    cJSON_AddItemToObject(root, "trueval", true_item);
    cJSON_AddItemToObject(sub, "number", num);
    cJSON_AddItemToObject(sub, "string", str);
    cJSON_AddItemToObject(sub, "rawdata", raw);
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t to_copy = printed ? (strlen(printed) < 511 ? strlen(printed) : 511) : 0;
    memcpy(json_copy, printed ? printed : "", to_copy);
    json_copy[to_copy] = '\0';
    cJSON_Minify(json_copy);
    has_sub = cJSON_HasObjectItem(root, "sub");
    raw_flag = cJSON_IsRaw(raw);
    array_item = cJSON_GetArrayItem(arr, 0);
    char *first_val = cJSON_GetStringValue(array_item);
    size_t sc_len = first_val ? (strlen(first_val) < 511 ? strlen(first_val) : 511) : 0;
    memcpy(scratch, first_val ? first_val : "", sc_len);
    scratch[sc_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(json_copy);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}