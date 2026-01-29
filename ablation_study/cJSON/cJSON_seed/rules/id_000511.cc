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
//<ID> 511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    const char json[] = "{\"flag\":false,\"numbers\":[1,2,3]}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *newnum = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool replace_result = 0;

    // step 2: Setup / Configure
    arr_size = cJSON_GetArraySize(numbers);
    newnum = cJSON_CreateNumber((double)arr_size + 10.0);
    cJSON_AddItemToArray(numbers, newnum);
    replace_result = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", true_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : 'x';
    buffer[1] = (char)('0' + (char)(arr_size % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}