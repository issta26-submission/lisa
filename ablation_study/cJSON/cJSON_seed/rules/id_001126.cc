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
//<ID> 1126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char json[] = "{\"root\":{\"child\":\"old_value\"},\"arr\":[\"x\",\"y\"]}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *root_obj = NULL;
    cJSON *child_item = NULL;
    cJSON *new_false = NULL;
    cJSON *str_arr = NULL;
    const char *arr_items[] = {"one", "two", "three"};
    char *tmp_buf = NULL;
    cJSON *ref_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    root_obj = cJSON_GetObjectItem(root, "root");
    child_item = cJSON_GetObjectItem(root_obj, "child");
    new_false = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(root_obj, child_item, new_false);
    str_arr = cJSON_CreateStringArray(arr_items, 3);
    cJSON_AddItemToObject(root, "strings", str_arr);
    tmp_buf = (char *)cJSON_malloc(16);
    memset(tmp_buf, 'X', 5);
    tmp_buf[5] = '\0';
    ref_str = cJSON_CreateStringReference((const char *)tmp_buf);
    cJSON_AddItemToObject(root, "ref", ref_str);

    // step 3: Operate / Validate
    int arr_size = cJSON_GetArraySize(str_arr);
    const char *first_str = cJSON_GetStringValue(cJSON_GetArrayItem(str_arr, 0));
    (void)arr_size;
    (void)first_str;

    // step 4: Cleanup
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}