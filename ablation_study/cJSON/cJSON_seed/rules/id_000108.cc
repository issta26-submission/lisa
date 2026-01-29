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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *n = cJSON_CreateNumber(123.0);
    char *tmp_buf = (char*)cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    char *printed = NULL;
    cJSON *got_arr = NULL;
    cJSON *first = NULL;
    cJSON *meta_item = NULL;
    cJSON_bool first_is_str = 0;
    cJSON_bool meta_is_str = 0;
    int count = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, n);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(root, "meta", "metadata");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    tmp_buf[0] = printed[0];
    got_arr = cJSON_GetObjectItem(root, "data");
    first = cJSON_GetArrayItem(got_arr, 0);
    first_is_str = cJSON_IsString(first);
    meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta");
    meta_is_str = cJSON_IsString(meta_item);
    count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_str);
    cJSON_AddBoolToObject(root, "meta_is_string", meta_is_str);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}