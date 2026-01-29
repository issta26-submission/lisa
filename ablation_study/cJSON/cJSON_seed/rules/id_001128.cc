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
//<ID> 1128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_names = NULL;
    cJSON *old_flag = NULL;
    cJSON *new_names = NULL;
    cJSON *new_flag = NULL;
    cJSON *elem = NULL;
    const char *json = "{\"names\":[\"a\",\"b\"],\"flag\":false}";
    const char *parse_end = NULL;
    const char *replacement_values[3] = { "alpha", "beta", "gamma" };
    const char **buffered_ptr = NULL;
    const char *extracted = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, (size_t)32, &parse_end, 1);
    old_names = cJSON_GetObjectItem(root, "names");
    old_flag = cJSON_GetObjectItem(root, "flag");
    new_names = cJSON_CreateStringArray(replacement_values, 3);
    new_flag = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(root, old_names, new_names);
    cJSON_ReplaceItemViaPointer(root, old_flag, new_flag);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(new_names);
    elem = cJSON_GetArrayItem(new_names, 0);
    extracted = cJSON_GetStringValue(elem);
    buffered_ptr = (const char **)cJSON_malloc(sizeof(const char *));
    memset((void *)buffered_ptr, 0, sizeof(const char *));
    *buffered_ptr = extracted;
    extracted = *buffered_ptr;

    // step 4: Cleanup
    cJSON_free((void *)buffered_ptr);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}