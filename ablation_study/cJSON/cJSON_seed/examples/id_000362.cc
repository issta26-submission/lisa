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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const colors_list[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *colors = cJSON_CreateStringArray(colors_list, 3);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    size_t buf_len = 32;
    char *tmp_buf = (char *)cJSON_malloc(buf_len);
    memset(tmp_buf, 0, buf_len);
    tmp_buf[0] = 'u'; tmp_buf[1] = 's'; tmp_buf[2] = 'e'; tmp_buf[3] = 'r'; tmp_buf[4] = ':'; tmp_buf[5] = ' '; tmp_buf[6] = 'A'; tmp_buf[7] = '\0';
    cJSON *user_str = cJSON_CreateString(tmp_buf);
    cJSON_AddItemToObject(root, "user", user_str);
    cJSON *static_name = cJSON_CreateString("static_name");
    cJSON_AddItemToObject(root, "name", static_name);

    // step 3: Operate & Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe);
    cJSON *flag_obj = cJSON_CreateNumber((double)root_is_obj);
    cJSON *flag_null = cJSON_CreateNumber((double)maybe_is_null);
    cJSON_AddItemToObject(root, "is_object_flag", flag_obj);
    cJSON_AddItemToObject(root, "is_null_flag", flag_null);

    // step 4: Cleanup
    cJSON_free(tmp_buf);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}