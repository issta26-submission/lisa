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
//<ID> 510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"obj\":{\"a\":1,\"arr\":[10,20,30],\"flag\":false},\"other\":2}";
    size_t len = strlen(json_text);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, len, &parse_end, 1);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *arr = cJSON_GetObjectItem(obj, "arr");

    // step 2: Setup / Configure
    cJSON *replacement_num = cJSON_CreateNumber(99.5);
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInObjectCaseSensitive(obj, "a", replacement_num);
    cJSON_ReplaceItemInObjectCaseSensitive(obj, "flag", replacement_true);

    // step 3: Operate / Validate
    int arr_size = cJSON_GetArraySize(arr);
    scratch[0] = (char)('0' + arr_size);
    scratch[1] = '\0';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}