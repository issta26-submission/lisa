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
//<ID> 651
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *minified_copy = NULL;
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("{\"x\":1}");
    cJSON *nested = cJSON_CreateObject();
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool has_obj = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON_AddItemToObject(obj, "arr", arr);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(nested, "raw", raw_item);
    cJSON_AddItemToArray(arr, nested);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(buffer, printed ? printed : "", to_copy);
    buffer[to_copy] = '\0';
    minified_copy = (char *)cJSON_malloc(printed_len + 1);
    memcpy(minified_copy, printed ? printed : "", printed_len + 1);
    cJSON_Minify(minified_copy);
    is_raw = cJSON_IsRaw(raw_item);
    has_obj = cJSON_HasObjectItem(root, "obj");
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(minified_copy);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}