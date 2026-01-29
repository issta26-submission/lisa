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
//<ID> 1194
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *str1 = cJSON_CreateString("first_element");
    cJSON_AddItemToArray(arr, str1);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, flag_false);
    cJSON *str_to_remove = cJSON_CreateString("will_be_removed");
    cJSON_AddItemToArray(arr, str_to_remove);

    // step 2: Configure
    char *raw_json = (char *)cJSON_malloc(64);
    strcpy(raw_json, "{ \"alpha\": 10, /* comment to remove */ \"beta\": 20 }");
    cJSON_Minify(raw_json);
    cJSON *min_parsed = cJSON_Parse(raw_json);
    cJSON_AddItemToObject(root, "minified_parsed", min_parsed);
    cJSON_free(raw_json);

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *str_after = cJSON_CreateString("after_delete");
    cJSON_AddItemToArray(arr, str_after);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}