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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root_obj = cJSON_CreateObject();
    cJSON *root_array = cJSON_CreateArray();
    cJSON *item_one = cJSON_CreateString("one");
    cJSON *item_shared = cJSON_CreateString("shared");
    cJSON *count_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *parsed_minified = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool is_invalid = 0;
    char json_buf[] = "  { \"hello\" : \"world\" }  ";

    // step 2: Setup
    cJSON_AddItemToArray(root_array, item_one);
    cJSON_AddItemToArray(root_array, item_shared);
    cJSON_AddItemReferenceToObject(root_obj, "myref", item_shared);

    // step 3: Operate
    arr_size = cJSON_GetArraySize(root_array);
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root_obj, "count", count_item);
    is_invalid = cJSON_IsInvalid(item_shared);
    bool_item = cJSON_CreateBool(is_invalid);
    cJSON_AddItemToObject(root_obj, "is_invalid_shared", bool_item);
    cJSON_Minify(json_buf);
    parsed_minified = cJSON_Parse(json_buf);
    cJSON_AddItemToObject(root_obj, "minified", parsed_minified);

    // step 4: Validate
    cJSON_IsInvalid(cJSON_GetObjectItem(root_obj, "minified"));
    cJSON_GetArraySize(root_array);

    // step 5: Cleanup
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);

    // API sequence test completed successfully
    return 66;
}