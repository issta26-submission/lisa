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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "  {  \"minified_key\"  :  null  }  ");
    cJSON_Minify(json_buf);
    cJSON *min_raw = cJSON_CreateRaw(json_buf);
    cJSON_AddItemToObject(root, "minified", min_raw);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, nul);
    cJSON *raw_num = cJSON_CreateRaw("123");
    cJSON_AddItemToArray(arr, raw_num);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first_item);
    cJSON *flag = cJSON_CreateBool(first_is_null);
    cJSON_AddItemToObject(root, "first_was_null", flag);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}