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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    const char json[] = "{\"alpha\":1,\"beta\":false}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *newnum = cJSON_CreateNumber(99.9);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "num", num1);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "num", newnum);
    int arraysz = cJSON_GetArraySize(arr);
    buffer[0] = (char)('0' + (char)(arraysz % 10));
    buffer[1] = (char)('0' + (char)(replaced));
    buffer[2] = json[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}