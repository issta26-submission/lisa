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
//<ID> 674
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *shared_num = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "shared", shared_num);
    cJSON *first_num = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, first_num);
    cJSON_AddItemReferenceToArray(arr, shared_num);
    cJSON *count_ptr = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    double new_count = cJSON_SetNumberHelper(count_ptr, 3.0);
    (void)new_count;
    (void)version;

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = 0;
    if (snapshot) {
        len = strlen(snapshot);
    }
    char *buffer = (char *)cJSON_malloc(len + 1);
    if (buffer) {
        memset(buffer, 0, len + 1);
    }
    if (snapshot && buffer) {
        memcpy(buffer, snapshot, len + 1);
    }
    if (buffer) {
        cJSON_Minify(buffer);
    }
    cJSON *parsed = NULL;
    if (buffer) {
        parsed = cJSON_Parse(buffer);
    }
    cJSON *parsed_arr = NULL;
    if (parsed) {
        parsed_arr = cJSON_GetObjectItemCaseSensitive(parsed, "array");
    }
    int parsed_size = 0;
    if (parsed_arr) {
        parsed_size = cJSON_GetArraySize(parsed_arr);
    }
    cJSON *parsed_first = NULL;
    double parsed_first_val = 0.0;
    if (parsed_arr && parsed_size > 0) {
        parsed_first = cJSON_GetArrayItem(parsed_arr, 0);
    }
    if (parsed_first) {
        parsed_first_val = cJSON_GetNumberValue(parsed_first);
    }
    (void)parsed_first_val;

    // step 4: Cleanup
    if (snapshot) {
        cJSON_free(snapshot);
    }
    if (buffer) {
        cJSON_free(buffer);
    }
    if (parsed) {
        cJSON_Delete(parsed);
    }
    if (root) {
        cJSON_Delete(root);
    }
    // API sequence test completed successfully
    return 66;
}