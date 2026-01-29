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
//<ID> 1226
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);

    // step 3: Operate
    const char *version = cJSON_Version();
    cJSON_bool is_array = cJSON_IsArray(arr);
    int buf_len = 128;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    const char *raw = " { \"a\": [ 1, 2, 3 ], \"b\": \"text\" } ";
    strcpy(buf, raw);
    cJSON_Minify(buf);
    char *printed = cJSON_Print(root);

    // step 4: Validate & Cleanup
    (void)version;
    (void)is_array;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}