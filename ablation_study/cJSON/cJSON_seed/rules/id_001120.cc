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
//<ID> 1120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"items\":[\"alpha\",\"beta\"],\"flag\":true}";
    const char *const new_strings[] = { "one", "two", "three" };
    const char *parse_end = NULL;
    size_t json_len = sizeof(json) - 1;
    size_t buf_size = 128;
    void *raw_buf = NULL;

    cJSON *root = NULL;
    cJSON *items_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *str_array = NULL;
    cJSON *false_item = NULL;
    cJSON *first_elem = NULL;
    const char *first_value = NULL;
    int new_array_size = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    items_item = cJSON_GetObjectItem(root, "items");
    flag_item = cJSON_GetObjectItem(root, "flag");
    str_array = cJSON_CreateStringArray(new_strings, 3);
    false_item = cJSON_CreateFalse();
    raw_buf = cJSON_malloc(buf_size);
    memset(raw_buf, 0, buf_size);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, items_item, str_array);
    cJSON_ReplaceItemViaPointer(root, flag_item, false_item);
    new_array_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "items"));
    first_elem = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "items"), 0);
    first_value = cJSON_GetStringValue(first_elem);
    (void)first_value;
    (void)new_array_size;

    // step 4: Cleanup
    cJSON_free(raw_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}